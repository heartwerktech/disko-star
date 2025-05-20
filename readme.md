# heartwerktech/esp32-generic-controller

- this project is released under GPLv3!

# Getting Started

## Prerequisites

- VSCode with PlatformIO

- checkout repository:

```
git clone --recurse-submodules https://github.com/heartwerktech/esp32-generic-controller.git
```

## Build
- create files in data/
  - "ssid.txt" with wifi name
  - "pass.txt" with wifi password
- select env (wemos_d1_mini32)
- Build, Upload
- Build Filesystem Image
- Upload Filesystem Image 


## FAQ / common problems
- make sure to have high enough current supply, otherwise MCU resets.
