#pragma once

#include <utility>
#include <vector>

namespace GLReady {

    class VertexBuffer
    {
    public:
        VertexBuffer &SetData(float *data, unsigned cntFloats);
        VertexBuffer &UpdateData(unsigned offsetFloats, unsigned newDataSizeFloats, float *newData);
        VertexBuffer &SetUsage(unsigned location, unsigned nFloats);
        VertexBuffer &EndSetUsage();

        void DrawTriangles(unsigned verticesOffset, unsigned verticesCnt);
        void DrawLines(unsigned verticesOffset, unsigned verticesCnt);

    private:
        unsigned _VAO, _VBO;

        bool _bufferCreated = false;
        unsigned _totalOffset = 0;
        unsigned _VBOSize = 0;
        std::vector<std::pair<unsigned, unsigned>> _verticesUsages;
    };

} // namespace GLReady
