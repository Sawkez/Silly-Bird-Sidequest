The recommended way to build Silly Bird Sidequest for any platform is with Docker.

1. Select <platform> from linux, windows, win32, haiku, web, psp, android;
2. Install [Docker](https://www.docker.com/products/docker-desktop/);
3. Clone the repo: ```git clone https://github.com/Sawkez/Silly-Bird-Sidequest --recurse-submodules && cd Silly-Bird-Sidequest```;
4. Create the docker image for your selected platform: ```build/create-docker-image.sh <platform>```;
5. Build: ```build/full-docker.sh <platform> [release | debug]``` (defaults to release);
6. Find the exported project in ```build/export/release/<platform>```.

Note: android does not currently support release builds.