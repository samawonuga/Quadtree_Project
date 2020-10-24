
#define MY_QUADTREE_H


struct quadTree;
typedef struct quadTree *quadTree;


struct quadTree{
    quadTree NW, SW, NE, SE;
    int xAxis, yAxis;
    int colour;
    int pixelWidth;
};



quadTree displayBlackNodes();
quadTree addNode();
