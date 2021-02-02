#ifndef TwoDBlockRenderer_H
#define TwoDBlockRenderer_H

class TwoDBlockRenderer
{
public:
    static TwoDBlockRenderer& GetInstance()
    {
        static TwoDBlockRenderer instance;
        return instance;
    }

    static unsigned int GetVBO()
    {
        return VBO;
    }

    static unsigned int GetVAO()
    {
        return VAO;
    }

private:
    TwoDBlockRenderer();

    TwoDBlockRenderer(TwoDBlockRenderer const&) = delete;
    void operator=(TwoDBlockRenderer const&) = delete;

    static unsigned int VBO, VAO, EBO;

    void CreateBlock();
};

#endif