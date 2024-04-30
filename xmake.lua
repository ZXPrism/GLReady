set_project("GLReady")

add_rules("mode.debug", "mode.release")
add_requires("glfw", "stb", "glm", "glad")
add_requires("imgui", {configs = { glfw = true, opengl3 = true }})

target("GLReadyDemo")
    set_languages("c99", "cxx17")
    set_kind("binary")
    set_warnings("all")

    add_includedirs("GLReady")
    add_files("GLReady/**.cpp")
    add_files("demo/**.cpp")
    add_packages("glfw", "stb", "glm", "imgui", "glad")

    after_build(function (target)
        os.cp(target:targetfile(), "bin/")
        os.cp("demo/shaders", "bin/")
    end)
target_end()
