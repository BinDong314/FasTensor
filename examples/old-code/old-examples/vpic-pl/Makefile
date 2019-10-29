AU_HOME=/global/homes/d/dbin/arrayudf-git-svn/build/

.PHONY:all
all:vpic-gradient vpic-xyz-global-position vpic-interpolate

vpic-gradient:vpic-gradient.cpp  
	CC -o vpic-gradient vpic-gradient.cpp -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF

vpic-interpolate:vpic-interpolate.cpp  
	CC -o vpic-interpolate vpic-interpolate.cpp -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF

vpic-xyz-global-position:vpic-xyz-global-position.cpp
	CC -o vpic-xyz-global-position vpic-xyz-global-position.cpp -I$(AU_HOME)/include -L$(AU_HOME)/lib -std=c++11 -std=gnu++11 -lArrayUDF

clean:
	rm vpic-gradient vpic-xyz-global-position vpic-interpolate
