#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <Python.h>
#include <string>
#include <filesystem>
#include "SZ3/api/sz.hpp"

using namespace std;
namespace fs = std::filesystem;

struct config{
    string N;
    string errorMode;
    string algorithem;
};

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
void compress(char *inPath, char *cmpPath, SZ3::Config conf) {
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

    printf("compression ratio = %.2f \n", conf.num * 1.0 * sizeof(T) / outSize);
    printf("compression time = %f\n", compress_time);
    printf("compressed data file = %s\n", outputFilePath);
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
                int binaryOutput, int printCmpResults) {

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
    if (printCmpResults) {
        //compute the distortion / compression errors...
        size_t totalNbEle;
        auto ori_data = SZ3::readfile<T>(inPath, totalNbEle);
        assert(totalNbEle == conf.num);
        SZ3::verify<T>(ori_data.get(), decData, conf.num);
    }
    delete[]decData;

    printf("compression ratio = %f\n", conf.num * sizeof(T) * 1.0 / cmpSize);
    printf("decompression time = %f seconds.\n", compress_time);
    printf("decompressed file = %s\n", outputFilePath);
}


void processCompression(char* argv[], const char* datPath, const char* filename, int N, struct config* Config){
    char inPath[100]; // 声明一个字符数组
    // 使用 strcpy 复制内容
    strcpy(inPath, datPath); // 使用 c_str() 获取 const char*
    char cmpPath[100]; //这个名字改成从pathList中原文件名，后缀是.dat.sz
     snprintf(cmpPath, sizeof(cmpPath), "/root/FAST/ComData/%s.sz", filename);
    char *conPath = nullptr;                      
    char decPath[100] ;
     snprintf(decPath, sizeof(cmpPath), "/root/FAST/deCompdata/%s.dat", filename);
    bool delCmpPath = false;
    int binaryOutput = 1; //是否输出二进制
    N = std::stoi(argv[3]);
    //Lorenzo/regression example :
    SZ3::Config conf(13761,1000); // dimension
    conf.cmprAlgo = SZ3::ALGO_INTERP_LORENZO;//ALGO_INTERP;ALGO_LORENZO_REG, 
    conf.lorenzo = false; // only use 1st order lorenzo
    conf.lorenzo2 = false;
    conf.regression = false;
    conf.regression2 = true;
    conf.errorBoundMode = SZ3::EB_REL; // EB_ABS,
    conf.absErrorBound = std::pow(10, -N); // absolute error bound 1e-3
    conf.relErrorBound = std::pow(10, -N);
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
    Config->N = to_string(N);
    Config->algorithem = algorithem;

    
    try{
        compress<float>(inPath, cmpPath, conf);
    }catch (const std::exception& e) {
        std::cerr << "Exception occurred in decompress: " << e.what() << std::endl;
        // 这里可以添加更多的错误处理代码
        // 例如，记录错误日志，释放资源，或者返回错误码等
    }
    cout<<"compres finish!"<<endl;
    try{
        decompress<float>(inPath, cmpPath, decPath, conf, binaryOutput, 1);
    }catch (const std::exception& e) {
        std::cerr << "Exception occurred in decompress: " << e.what() << std::endl;
        // 这里可以添加更多的错误处理代码
        // 例如，记录错误日志，释放资源，或者返回错误码等
    }
        
}


int main(int argc, char* argv[]){
    int errorBound = 5;
    struct config Config;
    const char* PathList[] = {
        "/root/LLM/VLBI_data/YNAO_CJ_70T700_241202000134s579_45k776Hz_47ms1859_R.dat",
        "/root/LLM/VLBI_data/YNAO_CJ_70T700_241202000221s767_45k776Hz_47ms1859_L.dat",
        "/root/LLM/VLBI_data/YNAO_CJ_70T700_241202000221s767_45k776Hz_47ms1859_R.dat",
        "/root/LLM/VLBI_data/YNAO_CJ_70T700_241202000308s953_45k776Hz_47ms1859_L.dat",
        "/root/LLM/VLBI_data/YNAO_CJ_70T700_241202000356s140_45k776Hz_47ms1859_L.dat",
        "/root/LLM/VLBI_data/YNAO_CJ_70T700_241202000356s140_45k776Hz_47ms1859_R.dat",
        nullptr
                            };
    const char* binaryDir = "/root/FAST/binaryData";
    size_t PathListLength = sizeof(PathList) / sizeof(PathList[0]) - 1;
    Py_Initialize(); 
    for(int i=0;i<PathListLength;i++){
        filesystem::path pathObj(PathList[i]);
        string filename_pure = pathObj.stem().string();
        processCompression(argv, PathList[i], filename_pure.c_str(), errorBound, &Config);
        
    }
    Py_Finalize();  


    return 0;
}
