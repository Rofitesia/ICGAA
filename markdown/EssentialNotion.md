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
  3. ​        $valid=true$
  4. ​        $for\ all\ points\ r\in P\ with\ r\ne p\ or\ q$    
  5. ​                $if\ (r\ lies\ to\ the\ left\ of\ the\ directed\ line\ from\ p\ to\ q):valid=false$
  6. ​        $if\ valied:\ add\ the\ directed\ edge\ \vec{pq}\ to\ E$
  7. $from\ E\ construct\ a\ list\ of\ vertices\ of\ convex\ hull(P),\ sorted\ in\ clockwise\ order$

* 注意事项：

  * 其中，第5步，判定一个点是否在向量左侧右侧的方法，数学库有O(1)时间复杂的实现，后期会讲到相关理论，此处暂不关注
  * 其中，最后一步，构造list时，从E中任取一个edge的起点放在list中，随后取出E中起点为上一个edge终点的edge，将其起点放进list中......

  * 其中，第5步完全可能出现，r在pq之上的情况，需要修改判断逻辑：r必须严格在pq右侧或者刚好在pq之上，才使得valid能够保持true。
  * 如果点的坐标是用浮点数来表示，并且三个点的坐标非常接近，由于计算机浮点误差可能会导致错误判断第5步的结果，使得list无法形成一个闭环，导致算法崩溃