#ifndef BlockRenderer_H
#define BlockRenderer_H

class BlockRenderer
{
public:
    static BlockRenderer& GetInstance()
    {
        static BlockRenderer instance;
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
    BlockRenderer();

    BlockRenderer(BlockRenderer const&) = delete;
    void operator=(BlockRenderer const&) = delete;

    static unsigned int VBO, VAO;

    void CreateBlock();
};

#endif