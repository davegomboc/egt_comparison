This code was used for performing the experiments reported upon in

    Dave Gomboc, Christian R. Shelton, Andrew S. Miner, and Gianfranco Ciardo
    (2024).  "Comparing Lossless Compression Methods for Chess Endgame Data."
    European Conference on Artificial Intelligence.

Please cite our publication if you make use of our code or academic paper.  At
the time of writing, a copy of the paper itself is online at
https://rlair.cs.ucr.edu/papers/pp/GomSheMinCia24.html.

This code has been licenced by its authors and copyright holders by the terms of
the "Apache License 2.0" (for which see the included LICENSE-2.0.txt file), with
the exception of the code inside the third_party subdirectory, which has its own
licencing terms specified therein.

At the time of writing, files generated in support of the experiments by this
and/or predecessor versions of this code are available online at
https://rlair.cs.ucr.edu/chess/.  Those files also include the Apptainer (nee
Singularity) container, which was the containerized environment used to perform
the experimental processing and testing, and the script
(bookworm-2024-08-hpcc.def) used to build it.  You may well need to modify bits
of it to match up with the Slurm HPC environment that your sysadmin provides.
An example of how to do so is included within the file itself.

Once you have made any necessary modifications, you can build the container from
a machine on which you have root access by invoking

    sudo apptainer build image.sif your-version.def

from within the directory in which your-version.def resides.

Then, migrate the generated image.sif (Singularity Image File) over to
the HPC cluster on which it will be used.  At the time of writing, for
cluster.hpcc.ucr.edu, the commands

    APPTAINER_IMAGE_DIR="${HOME}/apptainer_images"
    export APPTAINER_IMAGE_DIR
    singularity shell -B /usr/local/etc,/run/munge,.config "${APPTAINER_IMAGE_DIR}/image.sif"

should invoke the image, while on crisgc.engr.ucr.edu, the commands

    APPTAINER_IMAGE_DIR="${HOME}/apptainer_images"
    export APPTAINER_IMAGE_DIR
    singularity shell -B /etc/slurm,/run/munge,/data/ChristianShelton,/run/user "${APPTAINER_IMAGE_DIR}/image.sif"

should be able to be used.  Consult the Apptainer/Singularity documentation and
your sysadmin re: what launch command will work for the Slurm HPC environment
that you have access to.

Here is one way to get things set up once running within the container, which
includes setting some environment variables used by CMake and/or the compiled
code, from within a bash shell:

    GIT_TOP_DIR="${HOME}/gits"
    export GIT_TOP_DIR
    mkdir -p "${GIT_TOP_DIR}/egt_comparison/build_areas/clang++-git-linux"
    mkdir -p "${GIT_TOP_DIR}/egt_comparison/build_areas/g++-git-linux"
    mkdir -p "${GIT_TOP_DIR}/egt_comparison/main"
    pushd "${GIT_TOP_DIR}/gits/egt_comparison/main"
    rm -rf --one-file-system *
    git clone https://github.com/davegomboc/egt_comparison.git .
    EGT_DIR="${GIT_TOP_DIR}/egt_comparison/main/EGTs"
    export EGT_DIR
    SYZYGY_PATH="${EGT_DIR}/syzygy/3:${EGT_DIR}/syzygy/4:${EGT_DIR}/syzygy/5"
    export SYZYGY_PATH
    cd scripts
    ./prepare_repository_build_areas
    popd
    pushd "${EGT_DIR}"
    mkdir -p chesspresso/2 chesspresso/3 chesspresso/4 chesspresso/5
    mkdir -p cube_lists/2 cube_lists/3 cube_lists/4 cube_lists/5
    mkdir -p decision_dags/2 decision_dags/3 decision_dags/4 decision_dags/5
    mkdir -p decision_diagrams/2 decision_diagrams/3 decision_diagrams/4 decision_diagrams/5
    mkdir -p probe_report_nybbles/2 probe_report_nybbles/3 probe_report_nybbles/4 probe_report_nybbles/5

    # Create and populate "{$EGT_DIR}/syzygy/3/", "${EGT_DIR}/syzygy/4/", and
    # "${EGT_DIR}/syzygy/5/" with the appropriate .rtbw and .rtbz files.

    BUILD_AREA_DIR="${GIT_TOP_DIR}/egt_comparison/build_areas/clang++-git-linux/Release"
    export BUILD_AREA_DIR
    pushd "${BUILD_AREA_DIR}"
    ninja
    popd

If you would like to run the bundled tests, you can do so now by
invoking these additional commands:

    pushd "${EGT_DIR}/cube_lists"
    unxz --keep 2/K_vs_k.clst.xz 2/K_vs_k_D01I.clst.xz 3/K?_vs_k.clst.xz 3/K?_vs_k_D01I.clst.xz 4/K[NP]_vs_kp.clst.xz
    cd ../probe_report_nybble
    unxz --keep 2/*K_vs_k.wpr.xz 3/*K?_vs_k.wpr.xz 4/*K[NP]_vs_kp.wpr.xz 5/*KPP_vs_kp.wpr.xz
    popd
    pushd "${BUILD_AREA_DIR}"
    source/convert_main "${EGT_DIR}/cube_lists/2/K_vs_k.clst" "${EGT_DIR}/cube_lists/2/K_vs_k.icl"
    source/convert_main "${EGT_DIR}/cube_lists/3/KQ_vs_k.clst" "${EGT_DIR}/cube_lists/3/KQ_vs_k.icl"
    source/convert_main "${EGT_DIR}/cube_lists/3/KR_vs_k.clst" "${EGT_DIR}/cube_lists/3/KR_vs_k.icl"
    source/convert_main "${EGT_DIR}/cube_lists/3/KB_vs_k.clst" "${EGT_DIR}/cube_lists/3/KB_vs_k.icl"
    source/convert_main "${EGT_DIR}/cube_lists/3/KN_vs_k.clst" "${EGT_DIR}/cube_lists/3/KN_vs_k.icl"
    source/convert_main "${EGT_DIR}/cube_lists/3/KP_vs_k.clst" "${EGT_DIR}/cube_lists/3/KP_vs_k.icl"
    source/convert_main "${EGT_DIR}/cube_lists/2/K_vs_k_D01I.clst" "${EGT_DIR}/cube_lists/2/K_vs_k_D01I.icl"
    source/convert_main "${EGT_DIR}/cube_lists/3/KQ_vs_k_D01I.clst" "${EGT_DIR}/cube_lists/3/KQ_vs_k_D01I.icl"
    source/convert_main "${EGT_DIR}/cube_lists/3/KR_vs_k_D01I.clst" "${EGT_DIR}/cube_lists/3/KR_vs_k_D01I.icl"
    source/convert_main "${EGT_DIR}/cube_lists/3/KB_vs_k_D01I.clst" "${EGT_DIR}/cube_lists/3/KB_vs_k_D01I.icl"
    source/convert_main "${EGT_DIR}/cube_lists/3/KN_vs_k_D01I.clst" "${EGT_DIR}/cube_lists/3/KN_vs_k_D01I.icl"
    source/convert_main "${EGT_DIR}/cube_lists/3/KP_vs_k_D01I.clst" "${EGT_DIR}/cube_lists/3/KP_vs_k_D01I.icl"
    ninja test
    popd

Otherwise, the experiment should now be reproducible by running each
stage_[0-9][0-9]* bash script from the scripts directory in increasing
numerical order.  Some of these scripts rely upon "${SCRATCH}" within
the Slurm job environment resolving to speedy, temporary storage space
that is automatically cleared once a job exits.  If such a feature is
not available to you, then you may have to emulate this manually.

Here is one way to run the experiments.  If you do encounter unresolvable
difficulties, you can reach out to dave_gomboc@acm.org, who may or may not be
able to provide assistance.

    EXPERIMENT_OUTPUT_TOP_DIR="${GIT_TOP_DIR}/egt_comparison/main/experiments/2024-10_ECAI"
    export EXPERIMENT_OUTPUT_TOP_DIR
    mkdir -p "${EXPERIMENT_OUTPUT_TOP_DIR}"
    pushd "${EXPERIMENT_OUTPUT_TOP_DIR}"
    mkdir -p build_bdd chesspressoize clstize d1merge di dtrize
    mkdir -p dump_syzygy iclize osm probe_timings reduce
    popd
    pushd "${EGT_DIR}"
    mkdir -p sampled_positions
    cd sampled_positions
    mkdir -p 3 4 5
    popd
    pushd "${GIT_TOP_DIR}/egt_comparison/main/scripts"

    # Generate the .wpr files.
    ./stage_01_dump_syzygy_K_vs_k
    ./stage_02_dump_syzygy_Kx_vs_k
    ./stage_03a_dump_syzygy_Kx_vs_kx
    ./stage_03b_dump_syzygy_Kxx_vs_k
    ./stage_04a_dump_syzygy_Kxx_vs_kx_pawnful
    ./stage_04b_dump_syzygy_Kxx_vs_kx_pawnless
    ./stage_04c_dump_syzygy_Kxxx_vs_k

    # Generate the .epd files.
    #
    # If you want to use the exact .epd file that we generated, it is
    # available for download from https://rlair.cs.ucr.edu/chess/.
    # Or, you can generate your own (which is not guaranteed to be the
    # same) using stages 07-09c.
    ./stage_07_sample_Kx_vs_k
    ./stage_08a_sample_Kx_vs_kx
    ./stage_08b_sample_Kxx_vs_k
    ./stage_09a_sample_Kxx_vs_kx_pawnful
    ./stage_09b_sample_Kxx_vs_kx_pawnless
    ./stage_09c_sample_Kxxx_vs_k

    # Generate the .plb files from the .wpr files.  As of 2024-08, the
    # .plb format is almost obsolete.  We should really be able to
    # convert directly from .wpr to each of .clst, .dtr, and .mtbdd.
    # However, because such code hasn't yet been written, the .plb files
    # are generated only so that they can be used to generate the others.
    ./stage_11_chesspressoize_K_vs_k
    ./stage_12_chesspressoize_Kx_vs_k
    ./stage_13a_chesspressoize_Kx_vs_kx
    ./stage_13b_chesspressoize_Kxx_vs_k
    ./stage_14a_chesspressoize_Kxx_vs_kx_pawnful
    ./stage_14b_chesspressoize_Kxx_vs_kx_pawnless
    ./stage_14c_chesspressoize_Kxxx_vs_k

    # Generate the .clst files from the .plb files.
    ./stage_16_clstize_K_vs_k
    ./stage_17_clstize_Kx_vs_k
    ./stage_18a_clstize_Kx_vs_kx
    ./stage_18b_clstize_Kxx_vs_k
    ./stage_19a_clstize_Kxx_vs_kx_pawnful
    ./stage_19b_clstize_Kxx_vs_kx_pawnless
    ./stage_19c_clstize_Kxxx_vs_k

    # Generate the .dtr files from the .plb files.
    ./stage_21_dtrize_K_vs_k
    ./stage_22_dtrize_Kx_vs_k
    ./stage_23a_dtrize_Kx_vs_kx
    ./stage_23b_dtrize_Kxx_vs_k
    ./stage_24a_dtrize_Kxx_vs_kx_pawnful
    ./stage_24b_dtrize_Kxx_vs_kx_pawnless
    ./stage_24c_dtrize_Kxxx_vs_k

    # Generate the .icl files from the .clst files.
    ./stage_26_iclize_K_vs_k
    ./stage_27_iclize_Kx_vs_k
    ./stage_28a_iclize_Kx_vs_kx
    ./stage_28b_iclize_Kxx_vs_k
    ./stage_29a_iclize_Kxx_vs_kx_pawnful
    ./stage_29b_iclize_Kxx_vs_kx_pawnless
    ./stage_29c_iclize_Kxxx_vs_k

    # Apply distance-1 merging, then make irredundant to the cube lists.
    ./stage_31_d1merge_K_vs_k
    ./stage_32_d1merge_Kx_vs_k
    ./stage_33a_d1merge_Kx_vs_kx
    ./stage_33b_d1merge_Kxx_vs_k
    ./stage_34a_d1merge_Kxx_vs_kx_pawnful
    ./stage_34b_d1merge_Kxx_vs_kx_pawnless
    ./stage_34c_d1merge_Kxxx_vs_k

    # For n = 2, ... max(n) do {distance-n merge + make irredundant} where
    # max(2) = 13, max(3) = 11, max(4) = 9, max(5) = 5.  (You can try increasing
    # these values so long as you also make corresponding adjustments to later
    # scripts.)
    ./stage_36_di_K_vs_k
    ./stage_37_di_Kx_vs_k
    ./stage_38a_di_Kx_vs_kx
    ./stage_38b_di_Kxx_vs_k
    ./stage_39a_di_5fig_32GiB
    ./stage_39b_di_5fig_64GiB
    ./stage_39c_di_5fig_128GiB

    # Apply REDUCE.
    ./stage_41_reduce_K_vs_k
    ./stage_42_reduce_Kx_vs_k
    ./stage_43a_reduce_Kx_vs_kx
    ./stage_43b_reduce_KXX_vs_k
    ./stage_44a_reduce_Kxx_vs_kx_pawnful
    ./stage_44b_reduce_Kxx_vs_kx_pawnless
    ./stage_44c_reduce_Kxxx_vs_k

    # Apply RANDOM-EXPAND and IRREDUNDANT.
    ./stage_46_osm_K_vs_k
    ./stage_47_osm_Kx_vs_k
    ./stage_48a09_osm_Kx_vs_kx
    ./stage_48b09_osm_Kxx_vs_k
    ./stage_49a_osm_Kxx_vs_kx_pawnful
    ./stage_49b_osm_Kxx_vs_kx_pawnless
    ./stage_49c_osm_Kxxx_vs_k

    # Apply REDUCE.
    ./stage_51_xi_reduce_K_vs_k
    ./stage_52_xi_reduce_Kx_vs_k
    ./stage_53a_xi_reduce_Kx_vs_kx
    ./stage_53b_xi_reduce_Kxx_vs_k
    ./stage_54a_xi_reduce_Kxx_vs_kx_pawnful
    ./stage_54b_xi_reduce_Kxx_vs_kx_pawnless
    ./stage_54c_xi_reduce_Kxxx_vs_k

    # Generate four variants of the MTBDD files from the .plb files.
    # (Later, the code will use the variant that takes the least space
    # on disk, which is usually the two-sided matching variant.)
    ./stage_56_build_mtbdds_K_vs_k
    ./stage_57_build_mtbdds_Kx_vs_k
    ./stage_58a_build_mtbdds_Kx_vs_kx
    ./stage_58b_build_mtbdds_Kxx_vs_k
    ./stage_59a_build_mtbdds_Kxx_vs_kx_pawnful
    ./stage_59b_build_mtbdds_Kxx_vs_kx_pawnless
    ./stage_59c_build_mtbdds_Kxxx_vs_k
    popd

    # Combine the sample EPD files, each containing 2 million
    # positions from a single 3-, 4-, or 5-figurine material balance,
    # into a single, pseudo-randomly-ordered file that contains all
    # 290 million positions.
    pushd "${EGT_DIR}/sampled_positions"
    find 3 4 5 -name '*.epd' -exec cat {} + | shuf > shuffled-345-figurine.epd
    popd
