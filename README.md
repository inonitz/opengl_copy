[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![MIT][license-shield]][license-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
<h3 align="center">legacy opengl project</h3>

  <p align="center">
    C/C++ Simple Light Scene
    <br />
  </p>
</div>


<!-- ABOUT THE PROJECT -->
## About The Project
This project is an old VS Project I used in my high-school days to learn graphics programming<br>
Among other topics, I delved into SIMD, operator semantics in C++, memory allocation,<br>
linear algebra, abstraction of simple data structures, and so on<br>
Migrated the previous (unusable) Repository to my current **[workspace](https://github.com/inonitz/premake5-workspace-template)** 

### Project Structure
Same as my **[workspace](https://github.com/inonitz/premake5-workspace-template)** Project <br>


### Built With
<br> [<img height="100px" src="https://avatars.githubusercontent.com/u/255721?v=4">][glad-url] </br>
<br> 
  [![GLFW v3.4][GLFW.js]][GLFW-url]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  [![GLM][glm.js]][glm-url]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  [![Premake][Premake.js]][Premake-url]
</br>

<!-- GETTING STARTED -->
## Getting Started

### Prerequisites
* [premake](https://premake.github.io/docs/) 
* Working compiler toolchain, preferably clang
  * Windows: You should use [llvm](https://github.com/llvm/llvm-project/releases)
  * Linux:
      1. [installing-specific-llvm-version](https://askubuntu.com/questions/1508260/how-do-i-install-clang-18-on-ubuntu)
      2. [configuring-symlinks](https://unix.stackexchange.com/questions/596226/how-to-change-clang-10-llvm-10-etc-to-clang-llvm-etc)
  * Define these environment variables (in your PATH):
    * LLVMInstallDir
    * LLVMToolsVersion
* Powershell / Any Standard unix-shell


### Installation
```sh
git clone -b main https://github.com/inonitz/legacy-opengl-copy.git
# Don't forget to add your own remote repo
git remote set-url origin your_github_username/legacy-opengl-copy
git remote -v
```

<!-- USAGE EXAMPLES -->
## Usage

call ```premake5 help``` in the cloned repo directory ```(.vscode/..)```

### Common Commands:
```sh
    premake5 --proj=program cleanproj 
    premake cleanall 
    premake cleancfgs
    premake cleanclangd
    premake export-compile-commands
    premake --os=windows --arch=x86_64 --cc=clang gmake2
    premake --os=windows --arch=x86_64 --cc=clang vs2022
    premake --os=linux --arch=x86_64 --cc=clang gmake2
```



<!-- ROADMAP -->
## Roadmap
<br> None. This is a deprecated, archived project.         </br>
<br> It works, but should not be used for anything serious </br>


<!-- CONTRIBUTING -->
## Contributing
I'm not expecting any issues or pull-requests. if you feel that you must raise an issue/open a pull request, then sure


<!-- LICENSE -->
## License
Distributed under the MIT License. See `LICENSE` file for more information.


<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements
* [Kumodatsu](https://github.com/Kumodatsu/template-cpp-premake5/tree/master) For the initial template repo
* [Jarod42](https://github.com/Jarod42/premake-export-compile-commands/tree/Improvements) For the Improvements branch of export-compile-commands
* [Best-README](https://github.com/othneildrew/Best-README-Template)


<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/inonitz/premake5-workspace-template?style=for-the-badge&color=blue
[contributors-url]: https://github.com/inonitz/premake5-workspace-template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/inonitz/premake5-workspace-template?style=for-the-badge&color=blue
[forks-url]: https://github.com/inonitz/premake5-workspace-template/network/members
[stars-shield]: https://img.shields.io/github/stars/inonitz/premake5-workspace-template?style=for-the-badge&color=blue
[stars-url]: https://github.com/inonitz/premake5-workspace-template/stargazers
[issues-shield]: https://img.shields.io/github/issues/inonitz/premake5-workspace-template.svg?style=for-the-badge
[issues-url]: https://github.com/inonitz/premake5-workspace-template/issues
[license-shield]: https://img.shields.io/github/license/inonitz/premake5-workspace-template?style=for-the-badge
[license-url]: https://github.com/inonitz/premake5-workspace-template/blob/master/LICENSE
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/linkedin_username
[product-screenshot]: images/screenshot.png
[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white

[glad-url]: https://glad.dav1d.de/
[glbinding.js]: https://raw.githubusercontent.com/cginternals/glbinding/master/glbinding-logo.svg?sanitize=true
[GLFW-url]: https://github.com/glfw/glfw/releases/tag/3.4
[GLFW.js]: https://avatars.githubusercontent.com/u/3905364?s=200&v=4&size=150
[Premake-url]: https://github.com/premake/premake-core
[Premake.js]: https://avatars.githubusercontent.com/u/11135954?s=150&v=4
[glm-url]: https://github.com/g-truc/glm
[glm.js]: https://raw.githubusercontent.com/g-truc/glm/master/doc/manual/logo-mini.png
