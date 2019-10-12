//
// Created by Kevan on 2019-09-23.
//

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <omp.h>
#include <fstream>
#include <getopt.h>
#include <unistd.h>
//#include <set>
#include <memory.h>
#include <fstream>
//#include <vector>
#include "Bitmap.h"

#define MAX 17000000
using namespace std;

Bitmap* edge[MAX];

uint64_t set_intersection(Bitmap* map1,Bitmap* map2){
    uint64_t cnt = 0;
    int size = min(map1->maxpos,map2->maxpos);
    #pragma omp for
    for(int i = 0;i <= size;i++){
        if(get_bit(map1,i) & (get_bit(map2,i)))
            #pragma omp critical
            cnt++;
    }
    return cnt;
}

int main(int argc, char **argv)
{
    int oc;
    char *file;

    //命令行参数设置

    /*
    if((oc = getopt(argc,argv,"hf:")) == -1) {
        printf("Need to input the name of file!!!\n");
        exit(1);
    }
    */
    opterr = 0;
    while(oc != -1)
    {
        switch (oc){
            case 'h':
                printf("Help\n");
                break;
            case 'f':
                file = optarg;
                //printf("file name:%s\n",optarg);
                break;
            case '?':
                printf("No useful opt-parameter.\n");
                exit(1);
                break;
            case ':':
                printf("Please input the correct opt-parameter.\n");
                exit(1);
                break;
        }
        oc = getopt(argc,argv,"f:");
    }

    //printf("file name:%s\n",file);
    //初始化Bitmap
    for(int i = 0; i < MAX;i++)
    {
        edge[i] = init_bitmap_size(i);
    }


    //三角形计数
    /*memset(deg, 0, sizeof(deg));
    for (int i = 0; i < MAX; i++)
        edge[i].clear();
    */
    int sum = 0;  //用于统计三角形的个数
    int x[2];  //用于读取每两个的源节点，目的结点
    int maxn = 0, totalEdge = 0, maxd = 0, emaxd = 0;
       // 总边数       最大度数  最大有效度数
    int tmp;

    //file = (char*)"./Data/soc-LiveJournal1.bin";
    //第一遍遍历源文件得到node，edge，degree等信息
    ifstream inF;
    inF.open(file, ios_base::in | ios_base::binary);  //pos2
    while (!inF.eof()) {
        inF.read(reinterpret_cast<char*>(x), sizeof(int) * 2);
        if (inF.eof())
            break;
        tmp = max(x[0],x[1]);
        maxn = max(maxn,tmp);

        //cout << x[0] << ' ' << x[1] << endl;

        //deg[x[0]]++;
        //deg[x[1]]++;
        totalEdge += 1;

        if(x[0] > x[1]){
            //edge[x[0]].push_back(x[1]);
            if(!set_bit(edge[x[0]],x[1],1)){
                printf("Set bit error.\n");
                exit(1);
            }

        }
        else{
            //edge[x[1]].push_back(x[0]);
            if(!set_bit(edge[x[1]],x[0],1)){
                printf("Set bit error.\n");
                exit(1);
            }
        }
    }
    inF.close();

    /*/第二遍遍历得到图数据的有效邻接表存储
    inF.open(file, ios_base::in | ios_base::binary);  //pos3
    while (!inF.eof()) {
        inF.read(reinterpret_cast<char*>(x), sizeof(int) * 2);
        if (inF.eof())
            break;

        if (deg[x[0]] > deg[x[1]])
            edge[x[1]].push_back(x[0]);
        else if ((deg[x[0]] == deg[x[1]]) && (x[0] > x[1]))
            edge[x[1]].push_back(x[0]);
        else
            edge[x[0]].push_back(x[1]);

    }
    inF.close();
    */


    omp_set_num_threads(8);//8线程
    /*
    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i <= maxn; i++) {
            //去除效邻接表中的重复元素
            sort(edge[i].begin(), edge[i].end());  //对每个结点的邻接链表排序
            //vector<int>::iterator iter = unique(edge[i].begin(), edge[i].end());
            edge[i].erase(unique(edge[i].begin(), edge[i].end()), edge[i].end());
            //统计最大有效度数
            int s = edge[i].size();
            #pragma omp critical
            emaxd = max(emaxd, s);
        }
    }
    */

    #pragma omp parallel
    {
        #pragma omp for
        //计算三角形个数 Counting
        for (int i = 0; i <= maxn; i++) {
            //vector<int> Vtmp;
            uint64_t sz = edge[i]->maxpos;
            for (int j = 0; j <= sz; j++) {
                //int tmp = edge[i][j];
                //Vtmp = vec_intersection(edge[i], edge[tmp]);
                //int ssum = Vtmp.size();
                if(get_bit(edge[i],j)) {
                    uint64_t ssum = set_intersection(edge[i], edge[j]);
                    #pragma omp critical
                    sum += ssum;
                }
            }
        }
    }

    //cout << "结点数：" << maxn << endl;
    //cout << "边数(含重边)：" << totalEdge << endl;
    //cout << "最大度数：" << maxd << endl;
    //cout << "最大有效度数：" << emaxd << endl;
    //cout << "三角形个数为：" << sum << endl;



    //输出结果 Output
    printf("There are %d triangle in the input graph.\n",sum);

    return 0;
}