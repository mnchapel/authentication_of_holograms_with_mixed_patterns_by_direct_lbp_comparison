# Authentication of Holograms with Mixed Patterns by Direct LBP Comparison

The source code of the submitted paper "Authentication of Holograms with Mixed Patterns by Direct LBP Comparison".

## Build environment

The directory structure of the project and its build lifecycle is based on the [CMakeBaseCpp](https://github.com/josephgarnier/cpp-starter-project-cmake) project.

## Config

The folder `config` contains all the configurations needed to the code.
The file `input.yml` indicates the videos to use at the different step of the method:

- `ref_ovd`: the video used to compute the Multi LBP Models
- `train_ovd`: the video used to define the ellipse for the classification step
- `test_ovd`: the videos used to estimate if the hologram is genuine or counterfeit one

## Resources

<span style="color:red">/!\ IMPORTANT: the images given with this supplementary materials are confidential and can be used ONLY to test the code. As explained in the submitted paper, these data cannot be shared since they contain security measures to combat fraud.</span>

The folder `resources/` contains a subpart of our dataset. We have selected four videos of the hologram 0 of the French passport to test the code. The folder structure for each video is as follows:

- `<video_number>`: the id of the video
  - `metadata.yml`: the metadata of the video
  - `<hologram_number>`: the id of the hologram
    - `img/`: the folder that contains all the cropped images of the videos
    - `mask/`: the folder that contains all the masks
    - `crop_masks.yml`: the crop masks to create the Multi LBP Models
    - `filenames.yml`: the images filenames
    - `labels.yml`: the labels used to create the Multi LBP Models and to estimate the ellipse for the classification step.
    - `labels_test_<pattern_number>.yml`: the labels used to computed the precision and the recall.
    - `ovd_metadata.yml`: the metadata of the hologram.

According to the `config/input.yml` file, the videos are used as follows:

- `000`: used as the reference video to create the Multi LBP Models
- `001`: used as the train video to define the ellipse used during the classification step
- `002`: used as a test video. It is a video of a genuine hologram
- `003`: used as a test video. It is a video of a counterfeit hologram (scan version)

## Execution

If the reviewers do not want to compile the provided source code, an executable for Windows can be found in the `bin/` folder. Run the file `hologram_authentication.exe`.

## Compilation

The code has been compiled and tested on Windows 10 with VSCode, but the project is cross-platform. A build environment can also be generated for Linux and MacOS.

### 1. Requirements

The list of the requirements that we have used to compile the project are:

- CMake >= 3.20.0
- C++ Clang 13.0.0
- OpenCV 4.5.4
- Sciplot 0.2.2 (already provided in the `include/` folder, but you need to install gnuplot.)

### 2. Update the project dependencies

The file `source_code/cmake/project/DependenciesExternalOptions.cmake` contains paths to the libraries required by the project. Please update the following path:

- OpenCV: line 23, update the `OpenCV_DIR` path to your `OpenCVConfig.cmake` file.

For more information about project dependencies features can be found [here](https://github.com/josephgarnier/cpp-starter-project-cmake).

### 3. Update Cmake generator

Open the file `source_code/cmake/project/CMakeOptions.txt`, then edit the value of `GENERATOR_VAL` according to your build system. The list of available CMake generator can be found [here](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html).

### 4. Generate the project

Run the file `run-cmake.sh` (Linux, MacOS) or `run-cmake.bat` (Windows) to generate the build environment. It will be generated in the `build/` folder.

If you use VSCode, run the following tasks:

- `Generate Build Lifecycle with Clean Build Lifecycle`: to generate the build environment
- `Compile phase`: to compile the project
- `Execute main binary`: to run the executable

The results can be found in the `results/` folder. The structure folder is as follows:

- `<country_code>/`:
  - `<document_type>/`:
    - `ref_<document_label>_<video_name>/`
      - `ref_result/` the Multi LBP Models for each pattern
      - `train_<document_label>_<video_name>/`
        - `test_<document_label>_<video_name>/`: the images selected as the expected ones for each pattern
        - `train_result/`: the ellipse definition for each pattern
      - `authentication_report.yml`: a report of the authentication result for each video
      - `concise_camera_type_report.yml`: a report of the precision and recall results grouped by camera type
      - `concise_document_report.yml`: a report of the the precision and recall results grouped by document
      - `concise_pattern_report.yml`: a report of the the precision and recall results grouped by pattern
      - `detailed_report.yml`: a detailed report the precision and recall results for each video, where tp = true positive, tn = true negative, fp = false positive and fn = false negative
      - `mean_time_hologram_report.yml`: the mean time execution to compare one image with each Multi LBP Models
