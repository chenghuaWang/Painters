# Qt final project

A Qt final-project for zjgsu Desktop software development lecture.  **supervised by Chao.Song**

```
Author: chenghua.wang
Version:	0.2.1
```

**To other schoolmates:**
    You should follow the honor code, do not be a cv(copy and paste) expert.



## How to install ?

I just test this code on windows10, but I believe it can be compiled on other popular plateforms.

### Install from source

If you want to install this project from source code. You should follow the instruction below.

```
git clone https://github.com/chenghuaWang/Painters
git submodule update --init --recursive
```

Then, I highly recommend you to use Qt6 toolchain to compile it. You can use Qt Creator to compile it for simplify. The code has no compiler dependent, clang / MSVC / GNU is ok.

### Install from binary pakage

I will upload the binary package soon. The code is compiled on windows10 with Qt6, using MSVC. 

## Depedence

* I use the theme from [Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle](https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle)
* The Log sytem I used is [gabime/spdlog: Fast C++ logging library. (github.com)](https://github.com/gabime/spdlog)

##  What can this simple painter do ?

There r some sampels in `./test_samples` directory. You can open those samples by this painter. The sampels are also shown below.

![head_samples](./markdown_img/head_pic.png)

![samples](./markdown_img/brush_test.png)

![samples2](./markdown_img/text_cn_test.png)

![samples3](./markdown_img/text_en_emoji_test.png)

![samples4](./markdown_img/shape_type_test.png)

## LICENCE

![licence](https://www.gnu.org/graphics/lgplv3-with-text-154x68.png)

This project is licensed under version 3 of the GNU Lesser General Public License.
