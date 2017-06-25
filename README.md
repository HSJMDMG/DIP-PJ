# 数字图象处理期末PJ：阴影去除



Final Project For Digital Image Processing：shadow removal

具体的代码和说明见报告

C++代码是对[7]论文里贝叶斯方法的实现。论文里本来的效果就比较感人，效果比较一般。



## 运行环境

matlab部分的代码请自己下载。

C++代码是在CLion下开发的。入口main.cpp。需要实现安装opencv库，并且配好CMakeLists.txt



## 文件说明

#### report文件夹

报告和小论文。

#### src文件夹

``BayesianRefine.cpp`` 和``BayesianRefine.h`` 是 贝叶斯方法的实现，虽然代码可以跑可以用，但是实际效果并不好。

``HarmonizationRefine.cpp``和``HarmonizationRefine.h`` 是对论文[]里方法的尝试，但是没有成功写出来，论文里看起来效果很好，可以考虑尝试用来refine。

``IntensityDetector.cpp`` 和 ``IntensityDetector.h``是SSIP2011(见reference文件夹里的论文)阴影检测。可以使用。

``RoughRemoval.cpp`` 和``RoughRemoval.h`` 是一个普通的按照灰度值进行阴影移除算法。可能有BUG，不建议使用。

### reference文件夹

里面放了一些相关的文章，包含本文使用的方法，和一些比较robust的论文，参考下个部分：**相关的文章**



##   相关的文章：

### 1. 作者公开代码的论文：

[0] H. Gong and D. Cosker. Interactive shadow removal and ground truth for variable scene categories.

In BMVC 2014-Proceedings of the British Machine Vision Conference 2014. University of Bath,

2014.

[1] "[Interactive Shadow Removal and Ground Truth for Difficult Shadow Scenes](http://arxiv.org/abs/1608.00762)", Journal of the Optical Society of America (JOSA) A 2016. 【gong这个作者两篇文章效果都很不错。】

[2] R. Guo, Q. Dai, and D. Hoiem. Paired regions for shadow detection and removal. IEEE transactions

on pattern analysis and machine intelligence, 35(12):2956–2967, 2013. 【在训练集和测试集上效果拔群，但是自己拍的图片塞进去效果感人，几乎没detect对过】

### 2. 能够复现并且效果robust的方法：

[3]  Shadow detection and removal from a single image  SSIP 2011 【放在reference文件夹里了】(传统方法)

[4] Shor Y, Lischinski D. The Shadow Meets the Mask: Pyramid-Based Shadow Removal[C]// Computer Graphics Forum. 2008:577-586. 【在一些图上效果非常robust】

### 3. 本PJ主要用到的方法：

[3]的detection

[4]的detection 

[5] S. Darabi, E. Shechtman, C. Barnes, D. B. Goldman, and P. Sen. Image melding: Combining inconsistent images using patch-based synthesis. ACM Trans. Graph., 31(4):82–1, 2012. 【图像融合，可以找到Matlab代码】

[6] S. H. Khan, M. Bennamoun, F. Sohel, and R. Togneri. Automatic shadow detection and removal

from a single image. IEEE transactions on pattern analysis and machine intelligence, 38(3):431–446,2016.【贝叶斯】







