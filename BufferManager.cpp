#include "BufferManager.h"
BufferManager::BufferManager()
{

}

BufferManager::~BufferManager()
{
    for(Block* iter : Buffer_Pool)
    {
        delete iter;
    }
}

Block* BufferManager::find_block(const string file_name, const int offset)
{
    for(auto ele : Buffer_Pool)
    {
        if(file_name == ele->file_name && offset == ele->offset)
        {
            set_block_front(ele);
            return ele;
        }
    }
    int block_num = get_block_num();
    if(block_num < MAX_BLOCK)
    {
        Buffer_Pool.push_front(new Block);
        read_to_block(file_name, offset, Buffer_Pool.front());
    }
    else
    {
        if(Buffer_Pool.front()->is_pinned == true)
            return nullptr;
        else if(Buffer_Pool.front()->is_dirty == true)
        {
		    set_block_front(Buffer_Pool.back());
            write_to_file(Buffer_Pool.front());
        }
        read_to_block(file_name, offset, Buffer_Pool.front());
    }
    return Buffer_Pool.front();
}

void BufferManager::set_block_front(Block* moved_block)
{
    auto iter = Buffer_Pool.begin();
    for(auto ele : Buffer_Pool)
    {
        if(ele == moved_block)
            break;
        iter++;
    }
    Buffer_Pool.erase(iter);
    Buffer_Pool.push_front(moved_block);
}

void BufferManager::write_to_file(Block* block_to_write)
{
    block_to_write->is_pinned = false;
    fstream out(block_to_write->file_name, ios::out | ios::binary);
    out.seekp(block_to_write->offset, ios::beg);
    out.write(block_to_write->record, Block::BLOCK_SIZE);
    block_to_write->is_dirty = false;
    out.close();
    block_to_write->is_pinned = true;
}


void BufferManager::read_to_block(const string file_name, const int offset, Block* block_to_read)
{
    block_to_read->is_pinned = true;
    fstream in(file_name, ios::in | ios::binary);
    in.seekg(block_to_read->offset, ios::beg);
    in.read(block_to_read->record, Block::BLOCK_SIZE);
    in.close();
    block_to_read->file_name = file_name;
    block_to_read->offset = offset;
    block_to_read->is_pinned = false;
}
// void BufferManager::read_file(const string file_name)
// {
//     fstream in(file_name, ios::in | ios::binary);
//     int offset_to_end = ios::end;
//     int read_offset = 0;
//     while(!in.eof())
//     {
//         if(offset_to_end >= Block::BLOCK_SIZE)
//             read_offset = offset_to_end - Block::BLOCK_SIZE;
//         else
//             read_offset = offset_to_end;
//         Block* block = find_block(file_name, read_offset);
//         block->is_pinned = true;
//         block->offset = in.tellg();
//         in.read(block->record, Block::BLOCK_SIZE);
//         block->file_name = file_name;
//         set_block_front(block);
//         block->is_pinned = false;
//     }
//     in.close();
// }

const int BufferManager::file_block(string file_name)
{
    fstream in(file_name, ios::in | ios::binary);
    const int temp = (ios::end - ios::end) / Block::BLOCK_SIZE;
    in.close();
    return temp;
}

Block::Block():
    record(new char[BLOCK_SIZE]),
    offset(0),
    file_name(""),
    is_dirty(false),
    is_pinned(false)
{
    memset(record, 0, BLOCK_SIZE);
}

const int Block::get_record_length()
{
    for(int i = 0; i < BLOCK_SIZE; ++i)
        if(record[i] == 0)
            return i + 1;
    return BLOCK_SIZE;
}

Block::~Block()
{
    delete []record;
}

void Block::set_dirty()
{
    is_dirty = true;
}