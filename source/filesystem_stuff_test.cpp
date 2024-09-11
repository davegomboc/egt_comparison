#include "filesystem_stuff_test.hpp"


TEST(FilesystemStuff, CubeListDirectoryPathOptResolves) {
    auto const dir_path_opt(cube_list_directory_path_opt("K_vs_k"));
    EXPECT_TRUE(dir_path_opt);

    // (Just for convenience.)
    if (dir_path_opt) {
        std::cout << *dir_path_opt << std::endl;
    }
}


TEST(FilesystemStuff, PathForKVskCubeListObtained) {
    auto const path_opt(cube_list_path_opt("K_vs_k", 0, "", false));
    EXPECT_TRUE(path_opt);

    // (Just for convenience.)
    if (path_opt) {
        std::cout << *path_opt << std::endl;
    }
}


TEST(FilesystemStuff, PathForKVskD01ICubeListObtained) {
    auto const path_opt(cube_list_path_opt("K_vs_k", 1, "", false));
    EXPECT_TRUE(path_opt);

    // (Just for convenience.)
    if (path_opt) {
        std::cout << *path_opt << std::endl;
    }
}


TEST(FilesystemStuff, PathForKVskIndexedCubeListObtained) {
    auto const path_opt(cube_list_path_opt("K_vs_k", 0, "", true));
    EXPECT_TRUE(path_opt);

    // (Just for convenience.)
    if (path_opt) {
        std::cout << *path_opt << std::endl;
    }
}


TEST(FilesystemStuff, PathForKVskD01IIndexedCubeListObtained) {
    auto const path_opt(cube_list_path_opt("K_vs_k", 1, "", true));
    EXPECT_TRUE(path_opt);

    // (Just for convenience.)
    if (path_opt) {
        std::cout << *path_opt << std::endl;
    }
}
