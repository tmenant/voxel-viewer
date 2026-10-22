
#include <cpptrace/from_current.hpp>
#include "core/application.h"
#include "fmt/base.h"

int main()
{
    CPPTRACE_TRY
    {
        Application app("Voxel Viewer 3D", 1280, 720);
        app.run();
        return 0;
    }
    CPPTRACE_CATCH(const std::exception &e)
    {
        fmt::println("Exception: \"{}\"", e.what());
        cpptrace::from_current_exception().print();
    }
}
