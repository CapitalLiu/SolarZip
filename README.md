# SolarZip
SolarZip：An efficient and adaptive compression framework for Sloar EUV data

SolarZip achieving unprecedented compression ratios of up to **800×** for FSI data and **500×** for HRI data—surpassing traditional algorithms by 3-50×. 
Our two-stage evaluation framework ensures that compressed data remains suitable for critical scientific research.


## Compression performance

SolarZip demonstrated the overall best performance on the rate-distortion curve.
<img src="figure/combined_rate_distortion.png" alt="rate-distortion" style="width:70%;">

## Vision Example

### Full Solar Imager(FSI) dataset example:

<img src="figure/FSI_combined_results.png" alt="FSI-example" style="width:70%;">

### Hight Resolution Imager(HRI) dataset example:
<img src="figure/hri_vision_example.png" alt="HRI-example" style="width:70%;">

## Build Instructions

### requisites
Before you begin, ensure you have the following software and libraries installed:
* Python(3.8 or newer)
* Numpy
* Astropy
* Sunpy

You can install most dependencies using pip:
```
pip install numpy sunpy 
```

### Installation

1. Clone the repository to your local machine:
   ```
   git clone https://github.com/CapitalLiu/SolarZip.git
    ```
2. Navigate to the project directory and create build directory:
   ```
   cd SolarZip
   mkdir build && cd build
   ```
3. Execute the make command to compile the project
   ```
   cmake ..
   make -j8
   make install
   #The executable will now appear in the build/bin
   ```

### Usage

Command-line Syntax:
```
SolarZip -dim_num -bound_mode -bound_num <dataDir>
#compress files in the target directory.
```

Example Commands:
* Relative error bound mode
  ```
  ./bin/SolarZip 3 REL 1e-3 /SolarZip/data
  ```
* Absolute error bound mode
  ```
  ./bin/SolarZip 3 REL 1e-3 /SolarZip/data
  ```
  We have made the test data publicly available. If you're interested, you can access it here: 
https://github.com/CapitalLiu/SolarZip-TestData
## Citation
If you use SolarZip in your research, please cite our paper:
```
@misc{liu2025solarzip,
      title={SolarZip: An Efficient and Adaptive Compression Framework for Solar EUV Imaging Data}, 
      author={Zedong Liu and Song Tan and Alexander Warmuth and Frédéric Schuller and Yun Hong and Wenjing Huang and Yida Gu and Bojing Zhu and Guangming Tan and Dingwen Tao},
      year={2025},
      eprint={2504.13504},
      archivePrefix={arXiv},
      primaryClass={astro-ph.IM}
}
```

   
