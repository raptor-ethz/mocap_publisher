// FastDDS
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "DataStreamClient.h"
#include "Mocap_msg.h"
#include "Mocap_msgPubSubTypes.h"
#include "domain_participant.h"
#include "publisher.h"

// vicon helper functions
#include "vicon_helper.h"

#ifdef WIN32
#include <conio.h>   // For _kbhit()
#include <windows.h> // For Sleep()

#include <cstdio> // For getchar()
#else
#include <unistd.h> // For sleep()
#endif              // WIN32

#include <string.h>
#include <time.h>
