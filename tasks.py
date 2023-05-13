import os
from invoke import task


@task
def config(context, build_type="Release"):
    print(f"Configuring project with CMake (build_type: {build_type})...")
    build_dir = "build"
    os.makedirs(build_dir, exist_ok=True)

    cmd = [
        "cmake",
        f"-DCMAKE_BUILD_TYPE={build_type}",
        "-S",
        ".",
        "-B",
        build_dir
    ]

    print(" ".join(cmd))
    context.run(" ".join(cmd), pty=True)

    if not os.path.exists("compile_commands.json"):
        os.symlink(f"{build_dir}/compile_commands.json",
                   "compile_commands.json")


@task
def build(context):
    print("Building project with CMake...")
    build_dir = "build"

    cmd = [
        "cmake",
        "--build",
        build_dir,
        "-j"
    ]

    print(" ".join(cmd))
    context.run(" ".join(cmd), pty=True)


@task
def clean(context):
    if os.path.exists("build"):
        context.run("rm -rf build")

    if os.path.exists("compile_commands.json"):
        os.remove("compile_commands.json")
