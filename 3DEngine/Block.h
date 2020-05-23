#pragma once
enum BlockType
{
    BlockType_Grass = 0,
    BlockType_Ston = 1,
    BlockType_Water = 2,
    BlockType_Dirt,
    BlockType_Stone,
    BlockType_Wood,
    BlockType_Sand,

    BlockType_NumTypes,
};

class Block
{
public:
    Block();
    ~Block();

    bool IsActive();
    void SetActive(bool active);
    void SetBlockType(BlockType m_blockType);
    BlockType GetBlockType();
private:
    bool m_active = false;

    BlockType m_blockType;
};


