#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <Python.h>
#include <string>
#include <filesystem>
#include "SZ3/api/sz.hpp"
#include <chrono>
#include <vector>
#include <dirent.h> 
#include <sys/stat.h> 
#include "qcat/include/ByteToolkit.h"

#define MAX_FILES 2048
using namespace std;
namespace fs = std::filesystem;

struct config{
    string N;
    string errorMode;
    string algorithem;
};

// 辅助函数：设置Python路径为当前源文件所在目录
void setPythonPathFromSourceFile() {
    // 获取当前源文件的绝对路径
    std::string sourceFilePath = __FILE__;
    
    // 提取目录路径
    std::string::size_type pos = sourceFilePath.find_last_of("/\\");
    std::string directoryPath = (pos != std::string::npos) ? sourceFilePath.substr(0, pos) : ".";
    
    // 设置Python路径
    std::string pythonPathCommand = "import sys; sys.path.append('" + directoryPath + "')";
    PyRun_SimpleString(pythonPathCommand.c_str());
}

// 辅助函数：获取相对于源文件的数据目录路径
std::string getDataDirPath(const std::string& subDir) {
    // 获取当前源文件的绝对路径
    std::string sourceFilePath = __FILE__;
    
    // 提取源文件目录路径
    std::string::size_type pos = sourceFilePath.find_last_of("/\\");
    std::string sourceDir = (pos != std::string::npos) ? sourceFilePath.substr(0, pos) : ".";
    
    // 从源文件目录向上一级到项目根目录，然后进入data目录
    // 源文件在 tools/sz3/ 目录下，所以需要向上一级到 tools/，再向上一级到项目根目录
    std::string projectRoot = sourceDir + "/../../";
    std::string dataDir = projectRoot + "data/" + subDir;
    
    return dataDir;
}


std::string getDisplayPath(const std::string& absolutePath) {
    const std::string projectRoot = "/root/lzd/SolarZip/";
    
    if (absolutePath.find(projectRoot) == 0) {
        std::string relativePart = absolutePath.substr(projectRoot.length());
        
        if (relativePart.find("/../") != std::string::npos) {

            std::string resolvedPath = absolutePath;
            size_t pos;
            
            while ((pos = resolvedPath.find("/../")) != std::string::npos) {
                size_t prevSlash = resolvedPath.rfind('/', pos - 1);
                if (prevSlash != std::string::npos) {
                    resolvedPath = resolvedPath.substr(0, prevSlash) + resolvedPath.substr(pos + 3);
                } else {
                    break; 
                }
            }
            
            if (resolvedPath.find(projectRoot) == 0) {
                return resolvedPath.substr(projectRoot.length());
            }
        } else {
            return relativePart;
        }
    }
    
    return absolutePath;
}

const char** getFileList(const char* dirPath, int *i) {
    static const char* filePaths[MAX_FILES + 1]; 
    size_t count = 0; 

    DIR* dir = opendir(dirPath);
    if (!dir) {
        perror("Error opening directory");
        return NULL;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char fullPath[4096];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, entry->d_name);

        struct stat pathStat;
        if (stat(fullPath, &pathStat) == 0 && S_ISREG(pathStat.st_mode)) {
            // 只处理FITS文件
            char* filename = strrchr(fullPath, '/');
            if (filename == NULL) filename = (char*)fullPath;
            else filename++;
            
            char* ext = strrchr(filename, '.');
            if (ext != NULL && strcmp(ext, ".fits") == 0) {
                filePaths[count] = strdup(fullPath);
                count++;
            }

            // 防止超过最大文件数
            if (count >= 2048) {
                fprintf(stderr, "Warning: Too many files, some files are skipped.\n");
                break;
            }
        }
    }

    // 关闭目录
    closedir(dir);

    // 添加 NULL 作为结束标志
    filePaths[count] = NULL;
    *i = count;
    return filePaths;
}

std::string get_last_ten_chars(const char* path) {
    // 将const char*转换为path对象
    fs::path filePath(path);

    // 获取不带扩展名的文件名
    fs::path filename = filePath.stem();

    // 获取文件名的长度
    std::size_t len = filename.string().length();
    
    std::string last_five_chars;
    if (len <= 20) {
        last_five_chars = filename.string(); // 如果文件名长度小于等于5，直接使用整个文件名
    } else {
        // 提取文件名的最后十个字符
        last_five_chars = filename.string().substr(len - 20);
    }
    return last_five_chars;
}

template<class T>
void compress(char *inPath, char *cmpPath, SZ3::Config conf, compResult* Result) {
    T *data = new T[conf.num];

    SZ3::readfile<T>(inPath, conf.num, data);
    
    size_t outSize;
    SZ3::Timer timer(true);
    char *bytes = SZ_compress<T>(conf, data, outSize);
    double compress_time = timer.stop();

    char outputFilePath[1024];
    if (cmpPath == nullptr) {
        snprintf(outputFilePath, 1024, "%s.sz", inPath);
    } else {
        strcpy(outputFilePath, cmpPath);
    }
    SZ3::writefile(outputFilePath, bytes, outSize);
    double compress_ratio = conf.num * 1.0 * sizeof(T) / outSize;
    
    printf("compression ratio = %.2f \n", compress_ratio);
    printf("compression time = %f\n", compress_time);
    printf("compressed data file = %s\n", getDisplayPath(outputFilePath).c_str());
    Result->compression_ratio = compress_ratio;
    Result->compression_time = compress_time;
    try{
        delete[]data;
        delete[]bytes;
    }catch (const std::exception &e) {
        std::cerr << "Exception in compression: " << e.what() << std::endl;
    }

}

template<class T>
void decompress(char *inPath, char *cmpPath, char *decPath,
                SZ3::Config conf,
                int binaryOutput, int printCmpResults, compResult* Result) {

    size_t cmpSize;
    auto cmpData = SZ3::readfile<char>(cmpPath, cmpSize);
    if (!cmpData) {
        std::cerr << "Failed to read compressed data from " << cmpPath << std::endl;
        return;
    }
    if (cmpSize == 0) {
        std::cerr << "Failed to read compressed data or the size is zero." << std::endl;
        return;
    }

    SZ3::Timer timer(true);

    T *decData = nullptr;
    try {
        decData = SZ_decompress<T>(conf, cmpData.get(), cmpSize);
        if (!decData) {
            throw std::runtime_error("Decompression failed: SZ_decompress returned nullptr.");
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception in decompression: " << e.what() << std::endl;
        
    } catch (...) {
        std::cerr << "Unknown exception in decompression." << std::endl;
        return;
    }
    double compress_time = timer.stop();
    if (decData == nullptr) {
        std::cerr << "Decompression failed." << std::endl;
        return;
    }

    char outputFilePath[1024];
    if (decPath == nullptr) {
        snprintf(outputFilePath, 1024, "%s.out", cmpPath);
    } else {
        strcpy(outputFilePath, decPath);
    }
    if (binaryOutput == 1) {
        try {
            SZ3::writefile<T>(outputFilePath, decData, conf.num);
        } catch (const std::exception &e) {
            // 处理错误
            std::cerr << "Error: Failed to write file to " << outputFilePath << ". Exception: " << e.what() << std::endl;
            // 根据需要进行其他错误处理，如返回特定错误码或执行清理操作
        }
    } else {
        SZ3::writeTextFile<T>(outputFilePath, decData, conf.num);
    }

    Result->decompression_time = compress_time;
    delete[]decData;

    printf("compression ratio = %f\n", conf.num * sizeof(T) * 1.0 / cmpSize);
    printf("decompression time = %f seconds.\n", compress_time);
    printf("decompressed file = %s\n", getDisplayPath(outputFilePath).c_str());
}

void convertFitsToDat(const char* fitspath, const char* binaryDir, int index,long* naxes){
    
 
	//必须修改Python路径，否则会找不到Python模块
    PyRun_SimpleString("import sys");
	setPythonPathFromSourceFile();
 
	PyObject* pModule = NULL;
	PyObject* pFunc = NULL;
 
    //import模块
	pModule = PyImport_ImportModule("fits2bina");//模块文件名
    //找不到模块则报错
	if (pModule == nullptr) {
		printf("Failed to import fits2bina module\n");
		PyErr_Print();
		Py_Finalize();
        return;
	}

    PyObject* fits2binary = NULL;
	PyObject* args = NULL;

	fits2binary = PyObject_GetAttrString(pModule, "convert_fits_to_dat");
	//args = Py_BuildValue("(ss)", str1, str2); //设置传入Add的参数
	args = Py_BuildValue("(ssi)", fitspath, binaryDir, index); //path从pathlist中一个个获取
    PyObject* pRet = PyObject_CallObject(fits2binary, args); //调用了python函数将fits文件转化为dat

    if (!PyTuple_Check(pRet)) {
        cout << "The HDU "<<index<<" not need to compress!" << endl;
        PyErr_Print();  // 打印 Python 错误信息
        return;
    }
    //输出返回值
    Py_ssize_t tupleSize = PyTuple_Size(pRet);

    // 遍历元组并转换为 long
    for (Py_ssize_t i = 0; i < tupleSize; ++i) {
        PyObject* item = PyTuple_GetItem(pRet, i); // 获取元组中的每一项
        if (PyLong_Check(item)) { // 检查项是否为长整型
            naxes[i] = PyLong_AsLong(item); // 转换为 long
        } else {
            std::cerr << "Item is not a long." << std::endl;
            return;
        }
    }

}

void convertDatToFits(const char* fitspath, const char* decPath, struct config Config){

    /*调用Python将解压后的dat文件转化为Fits文件*/
	//必须修改Python路径，否则会找不到Python模块
    PyRun_SimpleString("import sys");
	setPythonPathFromSourceFile();
 
	PyObject* Module = NULL;
 
    //import模块
	Module = PyImport_ImportModule("fits2bina");//模块文件名
    //找不到模块则报错
	if (Module == nullptr) {
		printf("Failed to import fits2bina module in convertDatToFits\n");
		PyErr_Print();
		Py_Finalize();
		return;
	}

    PyObject* pCov = NULL;
	PyObject* arg = NULL;
    const char* str3 = decPath;
    string name = get_last_ten_chars(fitspath);
    string deCompDataDir = getDataDirPath("deCompData");
    string path = deCompDataDir + "/" + name + "_" + Config.algorithem + "_" + Config.errorMode + Config.N + ".fits";
	const char* str4 = path.c_str();



    pCov = PyObject_GetAttrString(Module, "convert_dat_to_fits");
    if (pCov == NULL) {
        PyErr_Print();  // 打印错误信息
        fprintf(stderr, "Error: 'convert_dat_to_fits' not found in module.\n");
        return;  // 或者返回适当的错误值
    }
    PyErr_Print();

	//args = Py_BuildValue("(ss)", str1, str2); //设置传入Add的参数
	arg = Py_BuildValue("(sss)", fitspath, str3, str4);
    if (arg == NULL) {
        PyErr_Print();
        fprintf(stderr, "Error: Failed to build arguments.\n");
        Py_DECREF(pCov);
        return;
    }
    PyErr_Print();
    PyObject_CallObject(pCov, arg); 
    PyErr_Print();
 
	
}

void processCompression(char* argv[], const char* resultPath, const char* filename, int N, struct config* Config, long* naxes,int index,compResult *Result){
    char inPath[512]; // 声明一个字符数组
    // 使用 strcpy 复制内容
    strcpy(inPath, resultPath); // 使用 c_str() 获取 const char*
    string compDataDir = getDataDirPath("compData");
    string binaryDataDir = getDataDirPath("binaryData");
    
    char cmpPath[512]; //这个名字改成从pathList中原文件名，后缀是.dat.sz
     snprintf(cmpPath, sizeof(cmpPath), "%s/%s_HDU%d.dat.sz", compDataDir.c_str(), filename,index);
    char *conPath = nullptr;                      
    char decPath[512] ;
     snprintf(decPath, sizeof(decPath), "%s/De_%s_HDU%d.dat", binaryDataDir.c_str(), filename,index);
    bool delCmpPath = false;
    int binaryOutput = 1; //是否输出二进制
    float E = std::stof(argv[3]);
    //Lorenzo/regression example :
    SZ3::Config conf(naxes[3], naxes[2], naxes[1], naxes[0]); // dimension
    conf.cmprAlgo = SZ3::ALGO_INTERP_LORENZO;//ALGO_INTERP;ALGO_LORENZO_REG, 
    conf.lorenzo = true; // only use 1st order lorenzo
    conf.lorenzo2 = true;
    conf.regression = true;
    conf.regression2 = true;
    conf.errorBoundMode = SZ3::EB_REL; // EB_ABS,
    conf.absErrorBound = E; // absolute error bound 1e-3
    conf.relErrorBound = E;
    //conf.relErrorBound = atof(relErrorBound);
    //conf.psnrErrorBound = atof(psnrErrorBound);
    //conf.l2normErrorBound = atof(normErrorBound);
    std::string algorithem;

    
    switch(argv[1][0]){
        case '1':
            conf.cmprAlgo = SZ3::ALGO_INTERP_LORENZO;
            algorithem = "AIL";
            break;
        case '2':
            conf.cmprAlgo = SZ3::ALGO_INTERP;
            algorithem = "AI";
            break;
        case '3':
            conf.cmprAlgo = SZ3::ALGO_LORENZO_REG;
            algorithem = "ALR";
            break;
        default:
            printf("Wrong input!/r");
            return;
    }

    const char* errorMode = argv[2];
    if(strcmp(errorMode,"REL")==0){
        conf.errorBoundMode = SZ3::EB_REL;
    }
    else{
        conf.errorBoundMode = SZ3::EB_ABS;
    }
    Config->errorMode = string(errorMode);
    Config->N = string(argv[3]);
    Config->algorithem = algorithem;

    
    try{
        compress<float>(inPath, cmpPath, conf, Result);
    }catch (const std::exception& e) {
        std::cerr << "Exception occurred in decompress: " << e.what() << std::endl;
        // 这里可以添加更多的错误处理代码
        // 例如，记录错误日志，释放资源，或者返回错误码等
    }
    cout<<"compres finish!"<<endl;
    try{
        decompress<float>(inPath, cmpPath, decPath, conf, binaryOutput, 1, Result);
    }catch (const std::exception& e) {
        std::cerr << "Exception occurred in decompress: " << e.what() << std::endl;
        // 这里可以添加更多的错误处理代码
        // 例如，记录错误日志，释放资源，或者返回错误码等
    }
    strcpy(Result->filePath,decPath);
    valuateData( inPath, decPath, naxes, Result);
        
}
struct pathConfig{
    string filename;
    string filename_pure;
    string datPath;
    char decPath[512];
    int hdu_num;
};
struct pathConfig processFile(const char* fitsPath,const char* binaryDir){
    struct pathConfig pc;
    filesystem::path pathObj(fitsPath);
    string filename = pathObj.filename().string();
    string filename_pure = pathObj.stem().string();
    string datPath = (std::filesystem::path(binaryDir)/(filename)).string();
    long naxes[4]={0};
    string binaryDataDir = getDataDirPath("binaryData");
    char decPath[512];//用原文件名dat后缀
    int status = 0;  // 状态变量
    int hdu_count = 0; // HDU 数量
    snprintf(decPath, sizeof(decPath), "%s/De_%s", binaryDataDir.c_str(), filename_pure.c_str()); 

    // 打开 FITS 文件
    PyRun_SimpleString("import sys");
	setPythonPathFromSourceFile();
    PyObject* Module = NULL;
    //import模块
	Module = PyImport_ImportModule("fits2bina");//模块文件名
    PyObject* pCov = NULL;
	PyObject* arg = NULL;
    if(!Module){
        printf("Failed to import fits2bina module in processFile\n");
        PyErr_Print();
        return pc;
    }

    pCov = PyObject_GetAttrString(Module, "count_hdu");
    arg = Py_BuildValue("(s)", fitsPath);
    PyObject* pRet = PyObject_CallObject(pCov, arg); 
    long hdu_long = PyLong_AsLong(pRet);
    hdu_count = static_cast<int>(hdu_long);


    pc.hdu_num = hdu_count;
    pc.filename = filename;
    pc.filename_pure = filename_pure;
    pc.datPath = datPath;
    strcpy(pc.decPath,decPath);
    return pc;
}

void compressFile(char *argv[], const char* fitsPath,const char* binaryDir, 
              struct config& Config,vector<compResult>& ResultVector){
    struct pathConfig pc= processFile(fitsPath,binaryDir);
        for(int j=0; j<pc.hdu_num; j++){
            long naxes[4]={0};
            convertFitsToDat(fitsPath, binaryDir, j, naxes);
            if(naxes[0]==0) continue;
            struct compResult Result; 
            string datPath = pc.datPath+"_HDU"+to_string(j)+".dat";
            processCompression(argv, datPath.c_str(), pc.filename_pure.c_str(), 5, &Config, naxes,j,&Result);
            ResultVector.push_back(Result);
        }
    //这里将hdu_nnum传给他，就知道有几个文件
    convertDatToFits(fitsPath,pc.decPath, Config);
}
//example： ./SZ3 1 REL 1e-3 /root/expeData/EUI_data/fsiData2
int main(int argc, char* argv[]){
    // WXAMPLE
    int errorBound = 5;
    struct config Config;
    std::vector<compResult> ResultVector;
    string binaryDataDir = getDataDirPath("binaryData");
    const char* binaryDir = binaryDataDir.c_str();
    char csvPath[512];
    const char* dataPath = argv[4];
    const char** PathList = nullptr;
    int count = 0;

    // 检测路径最后是否为 "testData"
    const char* testDataStr = "testData";
    int is_test = 0; // 默认标志为 0

    // 获取路径的最后部分
    const char* lastPart = strrchr(dataPath, '/'); // 查找最后一个 '/' 的位置
    if (lastPart != NULL) {
        lastPart++; // 跳过 '/'
    } else {
        lastPart = dataPath; // 如果没有 '/', 则整个字符串就是最后部分
    }

    // 比较最后部分是否为 "testData"
    if (strcmp(lastPart, testDataStr) == 0) {
        is_test = 1; // 如果是 "testData"，设置标志为 1
    }

    PathList = getFileList(dataPath,&count);
    Py_Initialize(); 
    for(int i=0;i<count;i++){
        compressFile(argv,PathList[i],binaryDir, Config, ResultVector);
    }
      //记录实验数据到csv文件
    if(is_test==0){
        string csvDataDir = getDataDirPath("");
        sprintf(csvPath, "%s/%s_%s.csv", csvDataDir.c_str(), Config.errorMode.c_str(),Config.N.c_str());
        FILE* csvFile = fopen(csvPath, "a");
        fprintf(csvFile, "filename,mode,tolerance,compression_ratio,compression_time,decompression_time,Min,Max,Range,MaxAbsoluteError,MaxRelativeError,MaxPWRelativeError,PSNR,NRMSE,NormErr,NormErrNorm,PearsonCoeff,SSIM\n");
        for(const compResult& Result : ResultVector){
            fprintf(csvFile,
        "%s,%s,%s,%.4f,%.2f,%.2f"
        "%.20G,%.20G,%.20G,%.10f,%f,%f,%f,%.20G,%f,%f,%f,%f\n",  
        Result.filePath,Config.errorMode.c_str(),Config.N.c_str(),
        Result.compression_ratio,Result.compression_time,Result.decompression_time,
        0.0,Result.Max,Result.Range,Result.MaxAbsoluteError,Result.MaxRelativeError,Result.MaxPWRelativeError,Result.PSNR,Result.NRMSE,Result.NormErr,Result.NormErrNorm,Result.PearsonCoeff,Result.SSIM
            );
        }
        fclose(csvFile);
    }
    Py_Finalize();  
    return 0;
}

