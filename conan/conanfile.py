from conans import ConanFile, CMake, tools

from os import path


class GlfwsandboxConan(ConanFile):
    """ Conan recipe for Glfwsandbox """

    name = "glfwsandbox"
    license = "MIT License"
    url = "https://github.com/Adnn/glfwsandbox"
    description = "Prototype and investigate GLFW usage."
    #topics = ("", "", ...)
    settings = ("os", "compiler", "build_type", "arch")
    options = {
        "build_tests": [True, False],
        "shared": [True, False],
        "visibility": ["default", "hidden"],
    }
    default_options = {
        "build_tests": False,
        "shared": False,
        "visibility": "hidden"
    }

    requires = (
        ("glad/0.1.34"),
        ("glfw/3.3.6"),
    )


    # Note: It seems conventionnal to add CMake build requirement
    # directly to the build profile.
    build_requires = ("cmake/3.22.0")

    build_policy = "missing"
    generators = "cmake_paths", "cmake_find_package", "CMakeToolchain"

    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto",
        "submodule": "recursive",
    }

    def _generate_cmake_configfile(self):
        """ Generates a conanuser_config.cmake file which includes the file generated by """
        """ cmake_paths generator, and forward the remaining options to CMake. """
        with open("conanuser_config.cmake", "w") as config:
            config.write("message(STATUS \"Including user generated conan config.\")\n")
            # avoid path.join, on Windows it outputs '\', which is a string escape sequence.
            config.write("include(\"{}\")\n".format("${CMAKE_CURRENT_LIST_DIR}/conan_paths.cmake"))
            config.write("set({} {})\n".format("BUILD_tests", self.options.build_tests))


    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake


    def configure(self):
        tools.check_min_cppstd(self, "11")


    def generate(self):
           self._generate_cmake_configfile()


    def build(self):
        cmake = self._configure_cmake()
        cmake.build()


    def package(self):
        cmake = self._configure_cmake()
        cmake.install()


    def package_info(self):
        pass
