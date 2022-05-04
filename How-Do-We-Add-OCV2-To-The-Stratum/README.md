How to add OCV2 algo to Stratum?

First, do the steps in this link and take a note of the flags it gives you.

https://github.com/ocvcoin/ocv2_algo/blob/master/README.md

Then copy the ocv2.cpp and ocv2.hpp files to the stratum/algos folder.

Edit the makefile in the stratum/algos directory and add the "ocv2.cpp" to the SOURCES section.

Edit the makefile in the stratum/algos directory and add the given flags to the CXXFLAGS section.


Edit the Makefile in the stratum directory and add the given flags to the LDFLAGS section. (linker will ignore unnecessary parts. do not worry)






Edit the stratum.cpp in the stratum directory and;




add the following code to the main function:
```
	if(ocv2_test_algo() != true)
		cerr << endl << "Error!! ocv2_test_algo() failed." << endl;	
```



add this to the g_algos array:
```
	{"ocv2", ocv2_hash, 1, 0, 0},
	
```


Edit the stratum.h in the stratum directory and;

add the following header:

```

#include "algos/ocv2.hpp"

```




Add your new algo to the web\yaamp\core\functions\yaamp.php file. There are up to three (3) places you may need to edit.


Review the algos table in the YIIMPFRONTEND database (this is your MariaDB/mysql server). Likely you will need to add an entry into this table using Phpmyadmin. Without it other parts if YIIMP won’t function and the algorithm won’t show on the home page.



Don't forget to create the ocv2.conf file.
