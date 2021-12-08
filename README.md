# OSTU

大津法（OTSU）是一种确定图像二值化分割阈值的算法，由日本学者大津于1979年提出。

## 流程

1. 用OTSU实现单阈值的分割（不用遗传算法）
2. 实现多阈值的分割（使用遗传算法）
   1. 设置分割为n个部分
   2. 确定直方图
   3. 初始化多种阈值
   4. 计算每种阈值的每种类的数学期望，类内方差；以及每个类之间的类间方差
   5. 求算单个类与其他类之间的类间方差的和（越大越好），除上类内方差（越小越稳定）得到比较值
   6. 将每个类的比较值都相加得到最后种与种直接比较的值
   7. 通过种与种间的值筛选出优的种（值越大越好）
   8. 变异随机生成新的阈值，优的阈值之间进行杂交（排序，对应取平均）
   9. 重复epoch step（4-7）

## Keep Learning

1. 

   ```c++
   cv::namedWindow("src", CV_WINDOW_NORMAL);
   cv::imshow("src", src);
   cv::waitKey();
   ```

   但是显示不出图片

   **解决方案**：后来哦，发现是自己文件的路径 \ 而不是 \\\\

2. **注释： 先CTRL+K，然后CTRL+C
   取消注释： 先CTRL+K，然后CTRL+U**

## 效果图

动漫图像因为色彩丰富，像素之间的跨度较大，不适合用该方法分割

![image]https://github.com/amoreyo/OSTU/blob/master/img/randm.png

![image]https://github.com/amoreyo/OSTU/blob/master/img/randm_dst.png

而真实的图像每一个像素都与之间的像素有所联系故较适合使用该方法分割

![image]https://github.com/amoreyo/OSTU/blob/master/img/winter.jpg

![image]https://github.com/amoreyo/OSTU/blob/master/img/winter_dst.jpg



## 待优化思路

1. 该算法中的超参数有：  
	```
   const int n = 4; 			// 要分割出的种类，此处为4种
   const int epoch = 6;		// 运行的次数
   const int rand_add = 3;		// 每次epoch中你要随机加入的种的个数
   const int all_node = 100;	// 最多会出现的node的个数
   ```
   因为我筛选的机制是，加上随机添加的3个种，计算每个种的var，sort（降序）之后删除后二    分之三，var临近的合并（故会产生n-1个新的种），这样粗略计算，最后会有 3+3=6;  6/3=2;  2+1=3; 3+3=6 的循环。  总之，超参数的选择是另一个很有意思的东西。

2. 因为是通过直方图来划分种类，这个问题很容易联想到其他类似K-means的解决方法

3. 既然是直方图，我们是否可以把总体的曲线模糊化，形成一条光滑的变化曲线，通过曲线表达式去直接得到每个波峰和波谷。 
   
   
   
