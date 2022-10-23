# libocv2

This repo contains libocv2 and sample miner.

You can easly add the ocv2 algorithm to your projects.

How to build & install libocv2 ?

This is very easy. 

Just run our bash script! (only works on ubuntu) (tested ubuntu versions: 16,18,20,21)

```

git clone https://github.com/ocvcoin/ocv2_algo.git

cd ocv2_algo && sudo bash build.sh

```


When the script is finished, it will give you the compiler flags so you can add it to the any project you want.




# How to add ocv2 algorithm to yiimp/Stratum?

Firstly, run the script above to install libocv2


After that...


Add the following code to the main function in the stratum/stratum.cpp file
```
	if(ocv2_test_algo() != 1)
		cerr << endl << "Error!! ocv2_test_algo() failed." << endl;	
```



Add the following code to the g_algos array in the stratum/stratum.cpp file
```
	{"ocv2", ocv2_hash, 1, 0, 0},
	
```



Add following header to stratum/stratum.h file
```

#include <algos/ocv2.hpp>

```




Add your new algo to the web\yaamp\core\functions\yaamp.php file. There are up to three (3) places you may need to edit.


Review the algos table in the YIIMPFRONTEND database (this is your MariaDB/mysql server). Likely you will need to add an entry into this table using Phpmyadmin. Without it other parts if YIIMP won’t function and the algorithm won’t show on the home page.



Don't forget to create the ocv2.conf file.
Don't forget to recompile stratum.

