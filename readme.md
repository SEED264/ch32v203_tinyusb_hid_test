This project depends on Platform.io.  
Please install Platform.io and the [ch32v platform](https://github.com/Community-PIO-CH32V/platform-ch32v).

### Note
Before compiling, you need to comment out the definition of SYSCLK_FREQ_96MHz_HSE in `%User%\.platformio\packages\framework-wch-noneos-sdk\System\ch32v20x\system_ch32v20x.h`.

tusb_config.h is copied from `tinyusb\examples\device\hid_composite\src`.