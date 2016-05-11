SoftBoundCETS for LLVM-3.8.0
============================

This is a README FILE for SoftBoundCETS pointer-based checking. For
more technical details and algorithms, visit SoftBoundCETS website at
http://www.cs.rutgers.edu/~santosh.nagarakatte/softbound/


Using SoftBoundCETS with LLVM+CLANG-3.8.0 on a x86-64 machine with Linux OS
===========================================================================


1. Download the github repository from https://github.com/santoshn/softboundcets-3.8.0.git

2. Build SoftBoundCETS for LLVM+CLANG 3.8.0

   1. Go to directory <git_repo>/llvm-38/ and execute the following commands
      ```
      mkdir build; cd build
      ```

   2. Configure and build LLVM, clang and softboundcets with the following commands
      ```
      cmake ..
      make -j8
      ```

3. Set up your environment to use SoftBoundCETS

   For example in bash, it would be
   ```
   export PATH=<git_repo>/llvm-38/build/bin:$PATH
   ```

4. Compile the SoftBoundCETS runtime library
   ```
   cd <git_repo>
   cd runtime
   make
   ```


5. Test whether it all worked

   1. Compile
      ```
      cd <git_repo>
      cd test
      clang -fsoftboundcets test.c -o test -L<git_repo>/runtime -lm -lrt -lsoftboundcets_rt
      ```

   2. Run the test program
      ```
      ./test
      ```

      Enter 10; the program executes successfully.

      Enter 105; a memory safety violation is triggered.

