#include <memory>
#include <GL\glew.h>
#include <variant>
#include <mutex>
class VertexBuffer {
    private:
        int bytesize;
        int size;
    public:
        GLfloat backgroundcolor[4];
        std::shared_ptr<GLfloat> buffer;
        VertexBuffer(GLfloat* input, int insize) {
            bytesize = insize*sizeof(GLfloat);
            size = insize;
            buffer = std::make_shared<GLfloat>(size);
            std::copy(input, input + size, buffer.get());
        }
        void setbuffer(GLfloat* input, int insize) {
            bytesize = size*sizeof(GLfloat);
            size = insize;
            buffer = std::make_shared<GLfloat>(size);
            std::copy(input, input + size, buffer.get());
        }
        int getbytesize() {
            return bytesize;
        }
        int getsize() {
            return size;
        }
};

class PlaceHolder {};

//layers variant typedef
typedef std::variant<int, VertexBuffer, PlaceHolder> Layer;


//render data container
class RenderData {
    private:
        int layercount = 0;    
    public:
        std::shared_ptr<Layer> layers;
        float maxfps = 60;
        std::mutex mtx;

        //constructors
        RenderData(Layer* inputlayers, int inputsize) {
            layercount = inputsize;
            layers = std::make_shared<Layer>(inputsize);
        }

        RenderData(){}

        int getlayercount() {
            return layercount;
        }

        void appendlayer(Layer layer) {
            layercount++;
            std::shared_ptr oldlayers = layers;
            layers = std::make_shared<Layer>(layercount);
            std::copy(oldlayers.get(), oldlayers.get() + layercount - 2, layers.get());
            layers.get()[layercount-1] = layer;
        }
        void addlayer(Layer layer, int pos) {
            layercount++;
            std::shared_ptr oldlayers = layers;
            layers = std::make_shared<Layer>(layercount);
            std::copy(oldlayers.get(), oldlayers.get() + pos - 2, layers.get());
            std::copy(oldlayers.get() + pos, oldlayers.get() + layercount - 2, layers.get());
        }
        void deletelayer();

        
};

//objects variant typedef
typedef PlaceHolder Object;

class PhysicsData {
    private:
        int objectcount;
        Object* objects;
    public:
        float tps;
};