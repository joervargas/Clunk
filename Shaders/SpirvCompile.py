import os
import shutil

cur_dir = os.getcwd()
print(cur_dir)
glsl_dir = "../Shaders/GLSL/"
hlsl_dir = "../Shaders/HLSL/"
spirv_dir = "../Shaders/SPIRV/"

glsl_list = os.listdir(glsl_dir)
print("GLSL Directory: \n\t" + str(glsl_list))
hlsl_list = os.listdir(hlsl_dir)
print("HLSL Directory: \n\t" + str(hlsl_list))


shader_compiler = shutil.which("glslangValidator")
if shader_compiler == "None":
    quit()


def get_spirv_name(filename):
    file_exts = filename.split(".")
    new_name = file_exts[0] + "." + file_exts[1] + ".spirv"
    return new_name

def compile_shaders(src_path, files):
    dest_path = spirv_dir
    options = ""
    if src_path == hlsl_dir:
        options = " -D -V -o "
    else:
        options = " -V -o "
    for f in files:
        source_file_path = src_path + f
        dest_file_path = dest_path + get_spirv_name(f)
        command = shader_compiler + options + dest_file_path + " " + source_file_path
        print("Src: " + source_file_path)
        print("Dest: " + dest_file_path)
        print("\t" + command)
        os.system(command)


if len(glsl_list) > 0:
    compile_shaders(glsl_dir, glsl_list)

# if len(hlsl_list) > 0:
#     compile_shaders(hlsl_dir, hlsl_list)
