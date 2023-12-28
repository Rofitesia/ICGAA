## Computational Geometry Introduction

### An Example : convex hulls

* convex ，一种平面上的子集S：集合中任意两点p，q，使得线段pq完全在集合之内。
* convex hull：包含S的最小convex。也是包含所有S的convex的交集
* 形象地说，一个convex hull，可以将平面上的点假设为钉在平板上的钉子，然后拿橡皮筋逐渐收缩包含范围，收缩之后形成的凸多边形的顶点即构成convex hull

#### algorithm：slow convex hull

* 慢速计算出凸包(convex hull)的算法
* 时间复杂度：O(3)
* 输入：平面上的点集P
* 输出：顺时针包含P的convex hull的顶点的list
* 算法流程：
  1. $set\ E\{\}$
  2. $ for\ all\ pairs(p,q)\in P\times P\ with\ p\ne q$
  3. $\qquad valid=true$
  4. $\qquad for\ all\ points\ r\in P\ with\ r\ne p\ or\ q$    
  5. $\qquad \qquad if\ (r\ lies\ to\ the\ left\ of\ the\ directed\ line\ from\ p\ to\ q):valid=false$
  6. $\qquad if\ valied:\ add\ the\ directed\ edge\ \vec{pq}\ to\ E$
  7. $from\ E\ construct\ a\ list\ of\ vertices\ of\ convex\ hull(P),\ sorted\ in\ clockwise\ order$

* 注意事项：

  * 其中，第5步，判定一个点是否在向量左侧右侧的方法，数学库有O(1)时间复杂的实现，后期会讲到相关理论，此处暂不关注
  * 其中，最后一步，构造list时，从E中任取一个edge的起点放在list中，随后取出E中起点为上一个edge终点的edge，将其起点放进list中......

  * 其中，第5步完全可能出现，r在pq之上的情况，需要修改判断逻辑：r必须严格在pq右侧或者刚好在pq之上，才使得valid能够保持true。
  * 如果点的坐标是用浮点数来表示，并且三个点的坐标非常接近，由于计算机浮点误差可能会导致错误判断第5步的结果，使得list无法形成一个闭环，导致算法崩溃

#### algorithm: incremental convex hull

* 增量的、通过逐个点加入的方式构建convex hull的算法
* 时间复杂度O(nlogn)
* 输入：平面上的点集P
* 输出：顺时针包含P的convex hull的顶点的list
* 算法流程:
  1. $sort\ points\ by\ x\ and\ y\ (lexicographically),resulting\ in\ a\ sequence\ p_1\dots p_n$
  2. $put\ the\ points\ p_1\ and\ p_2\ in\ a\ list\ L_{upper},\ with\ p_1\ as\ the\ first\ point$
  3. $for\ i=3,n$
  4. $\qquad append\ p_i\ to\ L_{upper}$
  5. $\qquad \mathbf{while}\ L_{upper}\ contains\ more\ than\ two\ points\ \mathbf{and}\ the\ last\ three\ points\ in\ L_{upper}\ do\ not\ make\ a\ right\ turn$
  6. $\qquad \qquad\ delete\ the\ middle\ of\ the\ last\ three\ points\ from\ L_{upper}$
  7. $put\ tht\ points\ p_n\ and\ p_{n-1}\ in\ a\ list\ L_{lower},\ with\ p_n\ as\ the\ last\ point$
  8. $\mathbf{repeat\ 3-6\ for\ L_{lower},\ process\ condition\ of\ loop\ carefully}$
  9. $remove\ the\ first\ and\ last\ points\ in\ L_{lower}\ to\ avoid\ redundant\ record$
  10. $Append\ L_{lower}\ to\ L_{upper},\ and\ call\ the\ resulting\ list\ L$
  11. $return\ L$

* 注意事项：

  * 如果三个点判断是否是“右转”时，发现三点在一条直线上，则一样需要删除中间的点，循环进行判断
  * 如果存在浮点精度误差，三点位置非常接近，导致三点判断时产生了错误的结果，则可能会生成一个有凹陷的多边形的顶点序列。
    * 对比slow convex hull算法，该算法存在浮点精度时不会导致算法崩溃，而是产生一个存在“不明显”的凹陷的多边形顶点序列，鲁棒性较强
    * 较好的解决方案是，如果存在浮点精度误差，并且三点位置过于接近，应当考虑在一定范围舍入后将位置接近的点判定为相同的点。

  

### Degeneracies and Robustness

#### three phases in development of geometric algorithm

* 第一阶段：不考虑“退化”、“简并”、“特殊”情况，主要关注整体几何问题的定义，梳理常见情况的算法解决方案
* 第二阶段：将“退化”、“简并”、“特殊”情况纳入考虑范围内
  * 低级做法是写一堆的条件分支来描述各种各样的特殊情况
  * 推荐做法是想办法将“退化”、“简并”、“特殊”情况融入到通用解决思路中。比如
    * 在增量计算2D凸包顶点的算法中，通用情况只需要按照横坐标x进行排序即可，但是可能存在不同的点横坐标x相同，此时不需要增加大量分支判断单独处理点之间纵坐标y的大小关系，而是通过“扩展”、“升维”的方式，将按横坐标x排序，改成按照x、y的字典序排序，并不改变算法的整体流程和结构
  * 只有在完全定义、理解了第一阶段关注的几何问题的一般情况之后，才应该考虑特殊情况
* 第三阶段：实现
  * 需要考虑各种算法细节，比如集合元素的基本操作：“判断点是在直线的左边/右边”
    * 这些基本操作实际时间复杂度对算法的总体时间复杂度影响很大，如果没有现成可用的几何库，需要以尽可能快的方式实现这些基本操作
  * 需要考虑浮点计算误差带来的问题
    * 如果项目对于时间开销不怎么关注，可以使用具有精确浮点计算属性的数学库，这会带来很大的额外开销，但是保证结果完美
    * 如果项目无法负担额外的时间开销，应当采取incremental convex hull中的策略，通过改进算法，使得在存在浮点误差的情况下，得到的错误结果是符合预期的、可接受的。

