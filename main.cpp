#include <iostream>
#include <stdio.h>  //for fprintf stdout
#include <stdlib.h> //for atof
#include "TutorialConfig.h"

/*
#ifdef USE_MYMATH
#include "MathFunctions.h"
#endif
*/

#include "memglobal.h"

using namespace std;

int main(int argc, char *argv[]) {

    fprintf(stdout,"%s Version %d.%d\n",
            argv[0],
            Tutorial_VERSION_MAJOR,
            Tutorial_VERSION_MINOR);

    // 开启内存全局计数
        mg_start();
        //mg_free(0);

        int* p = (int*)malloc(16);

        p = (int*)calloc(12, 2);
        *p = 154;

        puts("就这样!");

        p = (int*)realloc(NULL, 6);
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