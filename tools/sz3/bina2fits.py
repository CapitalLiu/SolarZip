import numpy as np
from astropy.io import fits


def convert_dat_to_fits(old_fits_file,dat_file, fits_file, data_shape):

    # 打开旧的 FITS 文件以获取头信息
    with fits.open(old_fits_file) as old_hdulist:
        # 确保有足够的 HDU
        if len(old_hdulist) < 1:
            raise ValueError("The old FITS file does not contain any HDUs.")

        # 读取旧 FITS 文件的头信息
        header = old_hdulist[0].header

    # 从 .dat 文件读取数据
    with open(dat_file, 'rb') as file:
        binary_data = file.read()
        
    print(data_shape)
    new_shape = []
    for size in data_shape:
        if size == 0:
            break
        new_shape.append(size)
    print(new_shape)
    data_shape = tuple(new_shape)

    # 将二进制数据转换为 NumPy 数组
    data_array = np.frombuffer(binary_data, dtype=np.float32).reshape(data_shape)

    # 创建一个新的 FITS 文件，只有一个 HDU
    hdu = fits.PrimaryHDU(data=data_array,header=header)

    # 将数据写入 FITS 文件
    hdu.writeto(fits_file, overwrite=True)

    print(f"Successfully converted {dat_file} to {fits_file}.")


# 使用示例
if __name__ == "__main__":
    old_fits_file = r'../FAST/FitsData/cont_dev.fits'  # 替换为您的旧 FITS 文件路径
    # dat_file = "E:\Google_Install\FITS\sourceoutput.dat"# 替换为您的 .dat 文件路径
    dat_file = r"../FAST/binaryData/data0.dat"  # 替换为您的 .dat 文件路径
    fits_file = r'../FAST/FitsData/decom_file.fits'  # 输出的 FITS 文件名
    data_shape = (21,643,643)  # 替换为数据的实际形状

    convert_dat_to_fits(old_fits_file,dat_file, fits_file, data_shape)

# import numpy as np
# from astropy.io import fits
#
#
# def convert_data_and_header_to_fits(old_fits_file, data_file, output_fits_file):
#     # 打开旧的 FITS 文件以获取头信息
#     with fits.open(old_fits_file) as old_hdulist:
#         # 确保有足够的 HDU
#         if len(old_hdulist) < 1:
#             raise ValueError("The old FITS file does not contain any HDUs.")
#
#         # 读取旧 FITS 文件的头信息
#         header = old_hdulist[0].header
#
#     # 读取数据
#     data = np.fromfile(data_file, dtype=np.float32)
#
#     # 获取头信息中的形状
#     naxis1 = int(header['NAXIS1'])
#     naxis2 = int(header['NAXIS2'])
#     naxis3 = int(header['NAXIS3'])
#
#     shape = (21,643,643)  # 根据头信息获取形状
#     data = data.reshape(shape)
#
#     # 创建新的 FITS 文件
#     hdu = fits.PrimaryHDU(data=data, header=header)
#     hdu.writeto(output_fits_file, overwrite=True)
#
#     print(f"Successfully converted data from {data_file} and header from {old_fits_file} to {output_fits_file}.")
#
#
# # 使用示例
# if __name__ == "__main__":
#     old_fits_file = r'E:\Google_Install\cont_dev.fits'  # 替换为您的旧 FITS 文件路径
#     data_file = 'E:\DeepLearnig\pythonProject\path_to_saved_model\CNN2.0\data.dat'  # 替换为您的数据文件路径
#     output_fits_file = 'new_output_file.fits'  # 输出的 FITS 文件名
#
#     convert_data_and_header_to_fits(old_fits_file, data_file, output_fits_file)
