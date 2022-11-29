PROJECT_EXECUTABLE=Application
PROJECT_DIRECTORY=Projects
DISTRIBUTION_DIRECTORY=Distributions
DISTRIBUTION_OS=""

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    DISTRIBUTION_OS="Linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    DISTRIBUTION_OS="MacOS"
elif [[ "$OSTYPE" == "cygwin" ]]; then
    DISTRIBUTION_OS="Windows"
elif [[ "$OSTYPE" == "msys" ]]; then
    DISTRIBUTION_OS="Windows"
elif [[ "$OSTYPE" == "win32" ]]; then
    DISTRIBUTION_OS="Windows"
elif [[ "$OSTYPE" == "freebsd"* ]]; then
    DISTRIBUTION_OS="FreeBSD"
else
    echo "Unknown OS"
fi

LOCAL_DISTRIBUTION_DIRECTORY="./$DISTRIBUTION_DIRECTORY/$DISTRIBUTION_OS"

[ ! -d $LOCAL_DISTRIBUTION_DIRECTORY ] && mkdir -p $LOCAL_DISTRIBUTION_DIRECTORY

cd $LOCAL_DISTRIBUTION_DIRECTORY && cmake ../.. && make && cd "./$PROJECT_DIRECTORY/$PROJECT_EXECUTABLE" && "./$PROJECT_EXECUTABLE" && cd ../../..
