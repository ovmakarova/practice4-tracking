set -e

cmake_build_dir="../build_cmake"

function try {
# This function executes command and stops
# exectution if return status wasn't 0
    "$@"
    status=$?
    if [ $status -ne 0 ]; then
        echo "ERROR with '$@' in $dir"
        exit $status
    fi
    return $status
}

function Header {
    echo ""
    echo "*****************************************************"
    echo "$@"
    echo "*****************************************************"
    echo ""
}

function BuildCMakeProject {
    Header "Build common CMake project"

    dir=$cmake_build_dir
    mkdir -p $cmake_build_dir
    cd $cmake_build_dir
    try cmake -DCMAKE_BUILD_TYPE=Debug ../code
    try make
}

function GoogleTest {
    Header "Run all GoogleTest tests"

    for test in $(ls -1 ./bin/*-test)
    do
        Header "Testing $test"
        try $test
    done
}

function Clean {
    rm -rf $cmake_build_dir
}

function Main {    
    BuildCMakeProject
    GoogleTest
}

Main
