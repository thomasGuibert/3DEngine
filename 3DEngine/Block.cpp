#include "Block.h"

Block::Block()
{
}

Block::~Block()
{
}

bool Block::IsActive()
{
    return m_active;
}

void Block::SetActive(bool active)
{
    m_active = active;
}

void Block::SetBlockType(BlockType blockType)
{
    m_blockType = blockType;
}

BlockType Block::GetBlockType()
{
    return m_blockType;
}