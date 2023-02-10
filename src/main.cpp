#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int main() {
    {
        std::ifstream bannerFile("E:/Dev/spedit/bin/banner.txt");

        if (!bannerFile.is_open()) {
            std::cout << "Failed to open banner file.\n";
        } else {
            std::string line;
            while (std::getline(bannerFile, line)) {
                std::cout << line << "\n";
            }
        }
    }

    std::string projectDiretory, projectName;

    std::cout << "Create your projects easily with Spedit\n";

    while (true) {
        std::cout << "Directory for the project: > ";
        std::cin >> projectDiretory;

        if (!std::filesystem::create_directories(projectDiretory)) {
            if (std::filesystem::exists(projectDiretory)) {
                std::cout
                    << "Directory already exists. Choose a different path!\n";
            }
            std::cout << "Failed to create Directory '" << projectDiretory
                      << "' Choose a different path\n";
        } else {
            std::cout << "Successfully created project directory '"
                      << projectDiretory << "'.\n";
            break;
        }
    }

    projectName = projectDiretory.substr(projectDiretory.find_last_of("\\"));

    projectName.erase(projectName.begin());
    std::cout << projectName << "\n";

    std::cout << "==============================================\n";

    std::cout << "Choose the Language of your project\n";

    std::string languageChoice;
    std::cout << "C (Press 1) | C++ (Press 2)\n";
    std::cin >> languageChoice;
    while (languageChoice != "1" && languageChoice != "2") {
        std::cout << "C (Press 1) | C++ (Press 2)\n";
        std::cin >> languageChoice;
    }

    if (languageChoice == "1") {
        std::cout << "Creating C project.\n";
    } else {
        std::cout << "Creating C++ project.\n";
    }

    std::filesystem::create_directory(projectDiretory + "/src");
    std::filesystem::create_directory(projectDiretory + "/vendor");
    std::filesystem::create_directory(projectDiretory + "/scripts");
    std::filesystem::create_directory(projectDiretory + "/bin");

    std::cout << "==============================================\n";
    {
        std::string vendorDir;
        std::transform(vendorDir.begin(), vendorDir.end(), vendorDir.begin(),
                       ::tolower);
        while (vendorDir != "end") {
            std::cout
                << "Add vendor directories by github repository (user/repo) "
                   "(stop adding: type 'end'): > ";
            std::cin >> vendorDir;

            if (vendorDir == "end") break;

            uint32_t position = vendorDir.find("/");
            std::string repoName = vendorDir.substr(position);
            std::string systemCommand = "git clone https://github.com/" +
                                        vendorDir + " " + projectDiretory +
                                        "/vendor" + repoName;
            system(systemCommand.c_str());
        }
    }

    std::cout << "==============================================\n";

    std::vector<std::string> includeDirs, linkerDirs, linkingLibraries,
        vendorFiles, predefinedMacros;
    {
        std::string includeDirInput;
        std::transform(includeDirInput.begin(), includeDirInput.end(),
                       includeDirInput.begin(), ::tolower);
        while (includeDirInput != "end") {
            std::cout << "Add include directories "
                         "(stop adding: type 'end'): > ";
            std::cin >> includeDirInput;

            if (includeDirInput == "end") break;

            includeDirs.push_back(includeDirInput);
        }
    }
    std::cout << "==============================================\n";
    {
        std::string linkerDirInput;
        std::transform(linkerDirInput.begin(), linkerDirInput.end(),
                       linkerDirInput.begin(), ::tolower);
        while (linkerDirInput != "end") {
            std::cout << "Add linker directories "
                         "(stop adding: type 'end'): > ";
            std::cin >> linkerDirInput;

            if (linkerDirInput == "end") break;

            linkerDirs.push_back(linkerDirInput);
        }
    }
    std::cout << "==============================================\n";
    {
        std::string libraryFileInput;
        std::transform(libraryFileInput.begin(), libraryFileInput.end(),
                       libraryFileInput.begin(), ::tolower);
        while (libraryFileInput != "end") {
            std::cout << "Add libraries to link to "
                         "(stop adding: type 'end'): > ";
            std::cin >> libraryFileInput;

            if (libraryFileInput == "end") break;

            linkingLibraries.push_back(libraryFileInput);
        }
    }

    std::cout << "==============================================\n";
    {
        std::string vendorFileInput;
        std::transform(vendorFileInput.begin(), vendorFileInput.end(),
                       vendorFileInput.begin(), ::tolower);
        while (vendorFileInput != "end") {
            std::cout << "Add vendor files"
                         "(stop adding: type 'end'): > ";
            std::cin >> vendorFileInput;

            if (vendorFileInput == "end") break;

            vendorFiles.push_back(vendorFileInput);
        }
    }
    std::cout << "==============================================\n";
    {
        std::string predefinedMacroInput;
        std::transform(predefinedMacroInput.begin(), predefinedMacroInput.end(),
                       predefinedMacroInput.begin(), ::tolower);
        while (predefinedMacroInput != "end") {
            std::cout << "Add predefined macros"
                         "(stop adding: type 'end'): > ";
            std::cin >> predefinedMacroInput;

            if (predefinedMacroInput == "end") break;

            predefinedMacros.push_back(predefinedMacroInput);
        }
    }
    {
        std::ofstream buildScriptFile(projectDiretory + "/scripts/build.bat");

        buildScriptFile << "@echo off\n";
        buildScriptFile << "SetLocal EnableDelayedExpansion\n\n";
        buildScriptFile << "pushd src/\n";
        buildScriptFile << "set sourceFiles=\n";
        if (languageChoice == "1") {
            buildScriptFile << "for /R %%f in (*.c) do (\n";
        } else {
            buildScriptFile << "for /R %%f in (*.cpp) do (\n";
        }
        buildScriptFile << "    set sourceFiles=!sourceFiles! %%f\n)\n";
        buildScriptFile << "popd\n\n";

        buildScriptFile << "pushd ..\\\n";
        std::string includeDirsForClang;
        for (auto& includeDir : includeDirs) {
            includeDirsForClang += " -I" + includeDir;
        }
        buildScriptFile << "set includeDirs=" + includeDirsForClang + "\n";

        std::string vendorFilesForClang;
        for (auto& vendorFile : vendorFiles) {
            vendorFilesForClang += vendorFile + " ";
        }
        buildScriptFile << "set vendorFiles=" + vendorFilesForClang + "\n";

        std::string linkerDirsForClang;
        for (auto& linkerDir : linkerDirs) {
            linkerDirsForClang += "-L" + linkerDir + " ";
        }

        buildScriptFile << "set linkerDirs=" + linkerDirsForClang + "\n";

        std::string libraryFilesForClang;
        for (auto& libraryFile : linkingLibraries) {
            libraryFilesForClang += "-l" + libraryFile;
        }
        buildScriptFile
            << "set libraryFiles=" + libraryFilesForClang +
                   " -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 "
                   "-ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 "
                   "-lopengl32 -llibucrt" +
                   "\n";
        buildScriptFile
            << "set compilerFlags=-g -Wall -Werror -Wvarargs -o bin/" +
                   projectName + ".exe\n";

        std::string predefinedMacrosForClang;
        for (auto& predefinedMacro : predefinedMacros) {
            predefinedMacrosForClang += "-D" + predefinedMacro + " ";
        }

        buildScriptFile << "set preDefinedMacros=-D_CRT_SECURE_NO_WARNINGS " +
                               predefinedMacrosForClang + "\n";

        buildScriptFile << "echo Starting " << projectName
                        << " build process...\n";

        buildScriptFile << "popd\n";
        if (languageChoice == "1") {
            buildScriptFile
                << "clang %sourceFiles% %vendorFiles% %includeDirs% "
                   "%linkerDirs% "
                   "%libraryFiles% %compilerFlags% %preDefinedMacros%\n";
        } else {
            buildScriptFile
                << "clang++ %sourceFiles% %vendorFiles% %includeDirs% "
                   "%linkerDirs% "
                   "%libraryFiles% %compilerFlags% %preDefinedMacros%\n";
        }
        buildScriptFile << "IF %ERRORLEVEL% NEQ 0 (\n";
        buildScriptFile << "  echo Failed to build " << projectName
                        << " - error code: %ERRORLEVEL%\n";
        buildScriptFile << ")";

        buildScriptFile.close();
    }
    {
        if (languageChoice == "1") {
            std::ofstream mainFile(projectDiretory + "/src/main.c");

            mainFile << "#include <stdio.h>\n\n";
            mainFile << "int main(int argv, char** argc) {\n";
            mainFile << "   printf(\"Hello World\\n\");\n";
            mainFile << "   return 0;\n";
            mainFile << "}\n";
            mainFile.close();
        } else {
            std::ofstream mainFile(projectDiretory + "/src/main.cpp");

            mainFile << "#include <iostream>\n\n";
            mainFile << "int main(int argv, char** argc) {\n";
            mainFile << "   std::cout << \"Hello World\\n\";\n";
            mainFile << "   return 0;\n";
            mainFile << "}\n";
            mainFile.close();
        }
    }

    std::cout << "Finished creating project.\n";
}