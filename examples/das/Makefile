AU_HOME=/global/homes/d/dbin/arrayudf-git-svn/build/

.PHONY:all
all:das-correlation 

das-correlation:das-correlation.cpp  
	CC -o das-correlation das-correlation.cpp -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF -O3


clean:
	rm das-correlation
