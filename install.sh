#!/bin/bash


# functions
function clean_up {
  echo "Cleaning up"
  echo "  rm source/*.hpp *.o"
  rm src/*.hpp *.o
  printf "\n"
}


# variables
ARC_DIR="tmp_archives"
LIB_DIR="/usr/local/lib/meerkat"
INC_DIR="/usr/local/include/meerkat"
USER_HOME=$(eval echo ~${SUDO_USER})
BASHRC="$USER_HOME/.bashrc"


# libraries
LIBS=( "meerkat_argument_manager"
       "meerkat_file_manager"
       "meerkat_logger"
       "meerkat_random_generator"
       "meerkat_vector2"
       "meerkat_vector3"
       #"meerkat_temporal_network"
      );


# start installation
printf "\n----------------------\n- Installing meerkat -\n----------------------\n\n"


# compile
echo "Compile"
cp include/*.hpp src/

for lib in ${LIBS[*]}
do
  echo "  g++ -c src/$lib.cpp -O3";
  err=$( g++ -c src/$lib.cpp -O3 2>&1 >/dev/null | tee /dev/stderr );
  if [ "$err" != "" ]
  then
    echo $err;
    printf "\n";
    echo "Install failed at compiling '$lib'.";
    clean_up;
    exit;
  fi
done
printf "\n"



# create archive
echo "Create libraries"
if [[ "$OSTYPE" == "darwin"* ]]
then
  # Mac
  echo "  ar -r libmeerkat.a *.o"
  ar -r libmeerkat.a *.o
else
  # Linux
  echo "  ar cr libmeerkat.a *.o"
  ar cr libmeerkat.a *.o
fi
printf "\n"


# make directories if they don't exist
echo "Make directories"
mkdir 
if [ -d "$LIB_DIR" ]
then
  echo "  $LIB_DIR already exists"
else
  echo "  mkdir $LIB_DIR";
  mkdir $LIB_DIR;
fi
if [ -d "$INC_DIR" ]
then
  echo "  $INC_DIR already exists"
else
  echo "  mkdir $INC_DIR";
  mkdir $INC_DIR;
fi
printf "\n"


# copy fies
echo "Copy libraries and headers"
echo "  mv *.a $LIB_DIR"
mv *.a $LIB_DIR
echo "  cp include/*.hpp $INC_DIR"
cp src/*.hpp $INC_DIR
printf "\n"


# Adding to path
if [[ "$OSTYPE" == "darwin"* ]]
then
  # Mac
  export LIBRARY_PATH=\$LIBRARY_PATH:$LIB_DIR
  export CPLUS_INCLUDE_PATH=\$CPLUS_INCLUDE_PATH:$INC_DIR
else
  # Linux - update .bashrc
  bashrc_is_up_to_date="no";
  while read line
  do
    if [[ $line == "# Meerkat library and include paths" ]]
    then
      bashrc_is_up_to_date="yes";
    fi 
  done < ~/.bashrc
  if [[ $bashrc_is_up_to_date == "no" ]]
  then
    echo "Updating .bashrc"
    echo "  appending paths to .bashrc";
    echo "" >> $BASHRC;
    echo "# Meerkat library and include paths" >> $BASHRC;
    echo "LIBRARY_PATH=\$LIBRARY_PATH:$LIB_DIR" >> $BASHRC;
    echo "export LIBRARY_PATH" >> $BASHRC;
    echo "CPLUS_INCLUDE_PATH=\$CPLUS_INCLUDE_PATH:$INC_DIR" >> $BASHRC;
    echo "export CPLUS_INCLUDE_PATH" >> $BASHRC;
  else
    echo "  .bashrc is already up to date"
  fi
  printf "\n"
fi


# give permissions to read
echo "Set permissions"
echo "  chmod +r /usr/local/include/meerkat/*.hpp"
chmod +r /usr/local/include/meerkat/*.hpp
printf "\n"


# clean up
clean_up;


# success message
echo "Install was successful!"
printf "\n"
