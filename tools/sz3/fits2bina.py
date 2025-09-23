import numpy as np
from astropy.io import fits
import os

def count_hdu(fits_file):
    with fits.open(fits_file) as hdul:
        num_hdus = len(hdul)  # 获取数据单元的数量
        print(f"Found {num_hdus} HDUs in the FITS file.")
        return num_hdus


def convert_fits_to_dat(fits_file, dat_file, index):

    # 打开 FITS 文件
    filename = os.path.basename(fits_file)
    try:
        with fits.open(fits_file) as hdul:
            data = hdul[index].data
            print(f"get HDU {index} data!")
            
            # 检查数据类型
            if data is None:
                print(f"No data found in HDU {index}. Skipping...")
                return 0

            # 输出数据类型
            print(f"Data type of HDU {index}: {data.dtype}")

            # 检查数据类型
            if isinstance(data.dtype, np.dtype) and data.dtype.names is not None:
                # 情况 1: data 是结构化数组
                new_data = []
                new_shape = ()
                row = 0
                
                for field_name in data.dtype.names:
                    field = data[field_name]
                    if field.shape[0] <= 3:
                        return 0
                    
                    if np.issubdtype(field.dtype, np.number):
                        row += 1
                        # 检查是否需要转换为 float32
                        if field.dtype != np.float32:
                            field_f32 = field.astype(np.float32)
                            print(f"Field '{field_name}' converted from {field.dtype} to float32")
                        else:
                            field_f32 = field
                            print(f"Field '{field_name}' already float32")
                        
                        # 将转换后的字段数据加入 new_data
                        new_data.extend(field_f32)
                
                new_data = np.array(new_data, dtype=np.float32)
                new_shape = (row, field.shape[0])
                print(f"Final shape: {new_shape}")
                binary_data = new_data.tobytes()
                naxes = new_shape

            else:
                # 情况 2: data 不是结构化数组
                if not np.issubdtype(data.dtype, np.number):  # 如果不是数字类型
                    print(f"Data is not numeric type: {data.dtype}")
                    return 0
                
                # 检查是否需要转换为 float32
                if data.dtype != np.float32:
                    print(f"Converting data from {data.dtype} to float32")
                    data = data.astype(np.float32)  # 转换为 float32
                else:
                    print("Data already float32")
            
                current_shape = data.shape
                naxes = tuple(current_shape)     
        
                # 将数据转换为二进制格式
                data_flat = data.flatten()  # 将数据展平为一维数组
                binary_data = data_flat.tobytes()  # 转换为二进制
            
            # 生成文件名
            #file_name = f'{dat_file}/{filename}_HDU{index}.dat'
            file_name = f'{dat_file}/{filename}_HDU{index}.dat'
            if os.path.exists(file_name):
                print(f"File {file_name} already exists, returning existing naxes")
                return naxes
            
            # 写入 .dat 文件
            with open(file_name, 'wb') as output_file:
                output_file.write(binary_data)
            
            print(f"Shape of HDU {index} data: {data.shape}")
            print(f"Successfully converted HDU {index} to {file_name}")
            print(f"Final naxes: {naxes}")
            return naxes
            
    except Exception as e:
        print(f"错误：{e}")
        return 0
def convert_dat_to_fits(old_fits_file,dat_file, fits_file):
    print(old_fits_file)
    # 打开旧的 FITS 文件以获取头信息
    with fits.open(old_fits_file) as old_hdulist:
        # 确保有足够的 HDU
        hdu_num = len(old_hdulist)
        if hdu_num < 1:
            raise ValueError("The old FITS file does not contain any HDUs.")
        hdu_list = fits.HDUList()
        for index in range(hdu_num):
            data = old_hdulist[index].data
                      
            # 读取旧 FITS 文件的头信息
            header = old_hdulist[index].header
            dat_Path = f"{dat_file}_HDU{index}.dat"
            if data is None:
                print("data is none")
                data_array = None
              
            else:
                print("data is not none")
                # 从 .dat 文件读取数据
                try:
                    with open(dat_Path, 'rb') as file:
                        binary_data = file.read()
                    if isinstance(data.dtype, np.dtype) and data.dtype.names is not None:
                        print("file type is structured")
                        restored_f32_data = np.frombuffer(binary_data, dtype=np.float32)
                        data_array = np.empty(data.shape, dtype=data.dtype)

                        # 遍历 data 的每个字段，恢复数值字段和非数值字段
                        for field_name in data.dtype.names:
                            field = data[field_name]
                            if np.issubdtype(field.dtype, np.number):
                                # 数值字段使用新的 float32 数据填充
                                data_array[field_name] = restored_f32_data[:len(field)].astype(field.dtype)
                                print(f"file type is {field.dtype}")
                                restored_f32_data = restored_f32_data[len(field):]
                            else:
                                # 非数值字段使用原始数据填充
                                data_array[field_name] = field
                    else:
                        data_shape = tuple(data.shape)

                        # 将二进制数据转换为 NumPy 数组
                        print("file type is float32")
                        #data_array = np.frombuffer(binary_data, dtype=np.float32).reshape(data_shape)
                        data_array = np.frombuffer(binary_data, dtype=np.float32).astype(np.int16).reshape(data_shape)

                except (FileNotFoundError, OSError) as e:
                    print(f"HDU{index} Using old fits data instead.")
                    data_array = data  # 如果打开失败，使用 fits_data
            if isinstance(old_hdulist[index],fits.PrimaryHDU):
                hdu = fits.PrimaryHDU(data=data_array,header=header)
            elif isinstance(old_hdulist[index],fits.BinTableHDU):
                hdu = fits.ImageHDU(data=data_array,header=header)
            elif isinstance(old_hdulist[index],fits.BinTableHDU):
                hdu = fits.BinTableHDU(data=data_array,header=header)
            else:
                hdu = fits.ImageHDU(data=data_array,header=header)
            # 将数据写入 FITS 文件
            hdu_list.append(hdu)
             
        hdu_list.writeto(fits_file,overwrite=True) 
        print(f"Successfully converted {dat_file} to {fits_file}.")
      
def pAdd(a,b):
    c = a + b
    file_name = f'{b}/data0.dat'
  
    data = np.array([1.0, 2.0, 3.0, 4.0], dtype=np.float32)  # 创建一个浮点数组
    # 将数据展平为一维数组并转换为二进制格式
    binary_data = data.tobytes()
    try:
        with open(file_name, 'wb') as output_file:
            output_file.write(binary_data)
        print(f"Successfully written data to {file_name}.")
    except IOError as e:
        print(f"Error opening or writing to file: {e}")
    print(c)
    return a
  

# 使用示例
if __name__ == "__main__":
    fits_file = r'../FAST/FitsData/cont_dev.fits'  # 替换为您的 FITS 文件路径
    dat_file = r'../FAST/binaryData'
    convert_fits_to_dat(fits_file,dat_file)