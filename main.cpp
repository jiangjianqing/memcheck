#include <iostream>
#include <stdio.h>  //for fprintf stdout
#include <stdlib.h> //for atof
#include "TutorialConfig.h"
#include "DebugOutput.h"

/*
#ifdef USE_MYMATH
#include "MathFunctions.h"
#endif
*/

#include "memglobal.h"

using namespace std;

int main(int argc, char *argv[]) {

    mg_start();//注册退出检查函数

    fprintf(stdout,"%s Version %d.%d\n",
            argv[0],
            Tutorial_VERSION_MAJOR,
            Tutorial_VERSION_MINOR);
	DEBUG("中文测试 %d", 123);

    // 实验步骤是, 是申请内存, 在操作内存
        char* as = (char*)malloc(16);

        //mg_check(as);

        // 内存越界了
        char a = as[16];
        as[16] = 18;
        char b = as[16];
        mg_check(as);

        // 重新分配内存, 再次越界
        //as = (char*)mc_realloc(as, 15);
        //as[15] = 44;
        //mg_check(as);

        free(as);

    // 开启内存全局计数


        int* p = (int*)malloc(16);
        free(p);

        p = (int*)calloc(12, sizeof(int));
        *p = 154;

        puts("就这样!");

        p = (int*)realloc(p, 6*sizeof(int));
        int y = *p;
        //p = (int*)realloc(NULL, 6);
        //free(p);
        puts("测试这样行!");


	cout<<"HelloWorld"<<endl;
/*
	double inputValue = argc > 1 ? atof(argv[1]) : 3;
 
#ifdef USE_MYMATH
	double outputValue = mysqrt(inputValue);
#else
	double outputValue = sqrt(inputValue);
#endif

	fprintf(stdout,"The square root of %g is %g\n",
          inputValue, outputValue);
*/
	//提示按任意键退出
	//system("pause");

	return 0;

}
