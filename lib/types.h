#include <memory>
#include <GL\glew.h>
#include <variant>
#include <mutex>

namespace types {
        template <class T>
        struct ArrayHolder {
                std::unique_ptr<T[]> storedarray = nullptr;
                int size = 0;
                int bytesize = 0;
                ArrayHolder(T* inputarray, int inputsize);
                ArrayHolder();
                ArrayHolder(ArrayHolder<T>& inputarrayholder);
                ArrayHolder<T>& operator= (ArrayHolder<T>& inputarrayholder);
                ArrayHolder<T>& operator= (const ArrayHolder<T>& inputarrayholder);
                void append(T& input);
        };

        struct PlaceHolder {};

        //layers variant typedef
        typedef ArrayHolder<GLfloat> VertexBuffer;


        //render data container
        struct RenderData: ArrayHolder<VertexBuffer> {
                std::mutex mtx;
                int maxfps = 60;
        };

        //objects variant typedef
        typedef PlaceHolder Object;

        struct PhysicsData {
                int objectcount;
                Object* objects;
                float tps;
        };
}