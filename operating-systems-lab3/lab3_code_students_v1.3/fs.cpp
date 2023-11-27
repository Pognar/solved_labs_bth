#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include "fs.h"

/*Utility*/
template <typename T>
void print_anything(T input)
{
    std::cout << input << std::endl;
}

int FS::ff_blocks()
{
    int index = -1;
    disk.read(FAT_BLOCK, (uint8_t *)fat);
    for (int i = 2; i < BLOCK_SIZE / 2; i++)
    {
        if (fat[i] == FAT_FREE)
        {
            index = i;
            break;
        }
    }
    return index;
}

std::vector<std::string> FS::path(std::string input)
{
    std::vector<std::string> vec;
    char char_array[input.length() + 1];
    strcpy(char_array, input.c_str());

    char *token = strtok(char_array, "/");
    while (token != NULL)
    {
        if (token != ".")
            vec.push_back(token);
        token = strtok(NULL, "/");
    }

    return vec;
}

void FS::update_fat_table(int index, uint32_t size)
{
    disk.read(FAT_BLOCK, (uint8_t *)fat);
    int temp;
    for (int i = 0; i < size; i++)
    {
        temp = ff_blocks();
        fat[temp] = index;
        index = temp;
        if (i == size - 1)
        {
            fat[temp] = FAT_EOF;
        }
    }
    disk.write(FAT_BLOCK, (uint8_t *)fat);
}

int FS::find_index(std::string file_name)
{
    disk.read(cur_dir, (uint8_t *)w_dir);
    disk.read(FAT_BLOCK, (uint8_t *)fat);

    // write everything to the data block
    int temp_index_1 = -1;
    for (dir_entry entry : w_dir)
    {
        if (entry.file_name == file_name && entry.type == TYPE_FILE)
        {
            // disk.read(entry.first_blk, (uint8_t*)data_block);
            temp_index_1 = entry.first_blk;
        }
    }
    return temp_index_1;
}
int FS::find_index_dir(std::string file_name) {
    disk.read(cur_dir, (uint8_t *)w_dir);

    // write everything to the data block
    int temp_index_1 = -1;
    for (dir_entry entry : w_dir)
    {
        if (entry.file_name == file_name && entry.type == TYPE_DIR)
        {
            // disk.read(entry.first_blk, (uint8_t*)data_block);
            temp_index_1 = entry.first_blk;
        }
    }
    return temp_index_1;
}

// initialize filesystem
// Set all directories to null basically.
FS::FS()
{
    std::cout << "FS::FS()... Creating file system\n";
    dir_entry entry;
    std::string name = "";

    // write name to entry object
    std::strcpy(entry.file_name, name.c_str());
    entry.first_blk = 0;
    entry.size = 0;
    entry.access_rights = 0;
    entry.type = EMPTY;
    for (auto &directories : w_dir)
    {
        directories = entry;
    }
}

FS::~FS()
{
}

std::string FS::get_access(uint8_t access)
{
    std::string access_string = "";
    if (access & 0x04)
    {
        access_string += "r";
    }
    else
    {
        access_string += "-";
    }
    if (access & 0x02)
    {
        access_string += "w";
    }
    else
    {
        access_string += "-";
    }
    if (access & 0x01)
    {
        access_string += "x";
    }
    else
    {
        access_string += "-";
    }
    return access_string;
}

// formats the disk, i.e., creates an empty file system
int FS::format()
{
    std::cout << "FS::format()\n";
    cur_dir = ROOT_BLOCK;
    uint8_t clean[BLOCK_SIZE] = {0};
    for (int i = 0; i < BLOCK_SIZE / 2; i++)
    {
        disk.write(i, clean);
    }

    fat[ROOT_BLOCK] = EOF;
    fat[FAT_BLOCK] = EOF; 
    for (int i = 2; i < BLOCK_SIZE / 2; i++)
    {
        fat[i] = FAT_FREE;
    }
    disk.write(FAT_BLOCK, (uint8_t *)fat);


    dir_entry r_entry;
    std::string empty_name = "";
    std::strcpy(r_entry.file_name, empty_name.c_str());
    r_entry.access_rights = 0;
    r_entry.size = 0;
    r_entry.first_blk = 0;
    r_entry.type = EMPTY;
    for (int i = 0; i < BLOCK_SIZE / 64; i++)
    {
        w_dir[i] = r_entry;
    }

    for (int i = 0; i < sizeof(w_dir) / sizeof(w_dir[0]); i++)
    {
        std::cout << w_dir[i].file_name;
    }

    disk.write(ROOT_BLOCK, (uint8_t *)w_dir);

    return 0;
}

// create <filepath> creates a new file on the disk, the data content is
// written on the following rows (ended with an empty row)
int FS::create(std::string filepath)
{
    disk.read(FAT_BLOCK, (uint8_t *)fat);
    disk.read(cur_dir, (uint8_t *)w_dir);

    std::cout << "FS::create(" << filepath << ")\n";
    std::vector<std::string> path_vec = path(filepath);
    std::string name = path_vec.back();
    path_vec.pop_back();

    if (name.length() > 55)
    {
        std::cout << "File name to long: " << name << " is " << name.length() << " character long, must be under 55" << std::endl;
        return -1;
    }
    if (find_index(name) != -1)
    {
        std::cout << "File already exists: " << name << std::endl;
        return -1;
    }
    dir_entry entry;
    int index = ff_blocks();

    if (index == -1)
    {
        std::cout << "No free blocks" << std::endl;
        return -1;
    }
    entry.first_blk = index;

    std::strcpy(entry.file_name, name.c_str());
    entry.type = TYPE_FILE;
    entry.access_rights = READ | WRITE;

    std::string data = "", cur_input = "";
    char data_block[BLOCK_SIZE] = {0};

    while (std::getline(std::cin, cur_input))
    {
        if (cur_input.size() == 0)
        {
            break;
        }
        cur_input = cur_input + "\n";
        data = data + cur_input;
    }
    if (data.size() == 0) {
        std::cout << "File must have content" << std::endl;
        return -1;
    }

    int file_size = data.size();
    int n_chunks = (file_size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    int block_num = data.size() / BLOCK_SIZE;
    if (data.size() % BLOCK_SIZE != 0)
    {
        block_num++; // add 1 block
    }
    int count = 0;
    for (auto &directories : w_dir)
    {
        if (directories.type == EMPTY)
        {
            count++;
        }
    }
    if (count < block_num)
    {
        std::cout << "Not enough space" << std::endl;
        return -1;
    }

    int cur_block = entry.first_blk;
    for (int i = 0; i < block_num; i++)
    {                                         
        fat[cur_block + i] = ff_blocks() + 1; // set the FAT to the next free block
        disk.write(FAT_BLOCK, (uint8_t *)fat);
    }
    fat[cur_block + block_num - 1] = FAT_EOF; // set the last block to EOF
    disk.write(FAT_BLOCK, (uint8_t *)fat);

    int data_offset = 0;
    int counter_2 = 0;

    for (int i = 0; i < n_chunks; i++)
    {
        disk.read(FAT_BLOCK, (uint8_t *)fat);
        char data_block[BLOCK_SIZE] = {0};
        int block_size = std::min(BLOCK_SIZE, file_size - data_offset);
        for (int i = 0; i < block_size; i++)
        {
            data_block[i] = data[counter_2];
            counter_2++;
        }
        disk.write(cur_block, (uint8_t *)data_block);
        data_offset += block_size;
        if (fat[cur_block] != FAT_EOF)
        {
            cur_block = fat[cur_block];
        }
    }

    entry.size = file_size;
    int temp_index;
    int counter = 0;
    for (dir_entry a : w_dir)
    {
        if (a.type == EMPTY)
        {
            temp_index = counter;
            break;
        }
        counter++;
    }

    w_dir[temp_index] = entry;

    disk.write(cur_dir, (uint8_t *)w_dir);

    return 0;
}

// cat <filepath> reads the content of a file and prints it on the screen
int FS::cat(std::string filepath)
{
    uint8_t data_block[BLOCK_SIZE];
    disk.read(cur_dir, (uint8_t *)w_dir);
    int temp_index = -2;
    int size;
    for (dir_entry entry : w_dir)
    {
        if (entry.file_name == filepath && entry.type == TYPE_FILE)
        {
            if ((entry.access_rights & READ) != READ)
            {
                std::cout << "File does not have READ access" << std::endl;
                return -1;
            }
            disk.read(entry.first_blk, (uint8_t *)data_block);
            temp_index = entry.first_blk;
            size = entry.size;
        }
    }

    if (temp_index == -2)
    {
        std::cout << "File: " << filepath << " is not found or do not have read rights" << std::endl;
        return -1;
    }
    disk.read(FAT_BLOCK, (uint8_t *)fat);

    while (fat[temp_index] != FAT_EOF)
    {
        int counter = 0;
        disk.read(temp_index, (uint8_t *)data_block);

        for (int i = 0; i < BLOCK_SIZE; i++)
        {
            std::cout << data_block[i];
            counter++;
        }
        temp_index = fat[temp_index];
    }
    disk.read(temp_index, (uint8_t *)data_block);

    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        std::cout << data_block[i];
    }
    return 0;
}

//Used for showing what exist in a dir
int FS::ls()
{

    disk.read(cur_dir, (uint8_t *)w_dir);
    std::cout << std::left << std::setw(15) << "Name" << std::right << std::setw(10) << "Rights" << std::right << std::setw(10) << "Bytes" << std::right << std::setw(10) << "Type" << std::endl;
    for (int i = 0; i < BLOCK_SIZE / 64; i++)
    {
        std::string name = w_dir[i].file_name;
        if (w_dir[i].type != EMPTY && name != "/")
        {
            std::cout << std::left << std::setw(15) << w_dir[i].file_name << std::right << std::setw(10) << get_access(w_dir[i].access_rights);
            if (w_dir[i].size == 0)
            {
                std::cout << std::right << std::setw(10) << "-";
            }
            else
            {
                std::cout << std::right << std::setw(10) << w_dir[i].size;
            }            
            
            if (w_dir[i].type == TYPE_DIR)
            {
                std::cout << std::right << std::setw(10) << "dir" << std::endl;
            }
            else
            {
                std::cout << std::right << std::setw(10) << "file" << std::endl;
            }
        }
    }

    return 0;
}

// cp <sourcepath> <destpath> makes an exact copy of the file
// <sourcepath> to a new file <destpath>
int FS::cp(std::string sourcepath, std::string destpath)
{
    int temp_dir = cur_dir;
    std::vector<std::string> path_vec_s = path(sourcepath);
    std::string name_s = path_vec_s.back();
    path_vec_s.pop_back();

    int size;
    uint8_t data_block[BLOCK_SIZE];
    int temp_index = -2;
    
    
    if (path_vec_s.size() > 0) {
        temp_dir = find_dir(path_vec_s);
    }
    disk.read(temp_dir, (uint8_t *)w_dir);

    for (auto &directory : w_dir)
    {
        if (directory.file_name == name_s && directory.type == TYPE_FILE)
        {
            if ((directory.access_rights & WRITE) == WRITE) 
            {
                disk.read(directory.first_blk, (uint8_t *)data_block);
                temp_index = directory.first_blk;
                size = directory.size;
            }     
            else{
                print_anything("You do not have the correct access rights");
                return -1;
            }
        }
    }
    disk.write(temp_dir, reinterpret_cast<uint8_t *>(w_dir));

    if (temp_index == -2)
    {
        std::cout << "File not found: " << sourcepath << std::endl;
        return -1;
    }
    disk.read(FAT_BLOCK, (uint8_t *)fat);
    std::string data_string = "";

    while (fat[temp_index] != FAT_EOF)
    {
        disk.read(temp_index, (uint8_t *)data_block);

        for (int i = 0; i < BLOCK_SIZE; i++)
        {
            data_string += data_block[i];
        }
        temp_index = fat[temp_index];
    }
    disk.read(temp_index, (uint8_t *)data_block);
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        if (data_block[i] != '\0')
        {
            data_string += data_block[i];
        }
        else
        {
            break;
        }
    }

    //int temp_dir2 = cur_dir;
    int temp_dir3 = cur_dir;
    std::vector<std::string> path_vec_d = path(destpath);
    std::string name_d = path_vec_d.back();
    path_vec_d.pop_back();
    
    if (destpath[0] == '/') 
    { 
        cur_dir = 0; 
    }
    int temp_dir2 = cur_dir;
    if (path_vec_d.size() > 0) {
        temp_dir2 = find_dir(path_vec_d);
    }

    cur_dir = temp_dir2;
    int ind_d = find_index_dir(name_d);
    if (ind_d == -1) 
    {
        ind_d = find_index(name_d);
        if (ind_d != -1)
        {
            std::cout << "Cannot copy to existing file: " << destpath << std::endl;
            return -1;
        }
    }
    else 
    {
        temp_dir2 = ind_d;
        name_d = name_s;
    }
    disk.read(FAT_BLOCK, (uint8_t *)fat);
    disk.read(temp_dir2, (uint8_t *)w_dir);
    for (dir_entry dir : w_dir) {
        if (dir.file_name == name_d) 
        {
            std::cout << "file " << name_d << " already exists in destination" << std::endl;
            return -1;
        }
    }

    dir_entry entry;
    int free_index = ff_blocks();
    entry.first_blk = free_index;
    std::strcpy(entry.file_name, name_d.c_str());
    entry.type = TYPE_FILE;
    entry.access_rights = READ | WRITE;
    entry.size = data_string.size();

    int file_size = data_string.size();
    int n_chunks = (file_size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    int block_num = data_string.size() / BLOCK_SIZE;
    if (data_string.size() % BLOCK_SIZE != 0)
    {
        block_num++; // add 1 block
    }

    int cur_block = entry.first_blk;
    for (int i = 0; i < block_num; i++)
    {                                        
        fat[cur_block + i] = ff_blocks() + 1; // set the FAT to the next free block
        disk.write(FAT_BLOCK, (uint8_t *)fat);
    }
    fat[cur_block + block_num - 1] = FAT_EOF; // set the last block to EOF
    disk.write(FAT_BLOCK, (uint8_t *)fat);

    int data_offset = 0;
    int counter_2 = 0;

    for (int i = 0; i < n_chunks; i++)
    {
        disk.read(FAT_BLOCK, (uint8_t *)fat);
        char data_block[BLOCK_SIZE] = {0};
        int block_size = std::min(BLOCK_SIZE, file_size - data_offset);
        for (int i = 0; i < block_size; i++)
        {
            data_block[i] = data_string[counter_2];
            counter_2++;
        }
        disk.write(cur_block, (uint8_t *)data_block);
        data_offset += block_size;
        if (fat[cur_block] != FAT_EOF)
        {
            cur_block = fat[cur_block];
        }
    }

    entry.size = file_size;
    int temp_index_2;
    int counter = 0;
    for (dir_entry a : w_dir)
    {
        if (a.type == EMPTY)
        {
            temp_index_2 = counter;
            break;
        }
        counter++;
    }

    w_dir[temp_index_2] = entry;
    w_dir[temp_index_2].type == TYPE_FILE;

    disk.write(temp_dir2, reinterpret_cast<uint8_t *>(w_dir));
    cur_dir = temp_dir3;
    return 0;
}

// mv <sourcepath> <destpath> renames the file <sourcepath> to the name <destpath>,
// or moves the file <sourcepath> to the directory <destpath> (if dest is a directory)
int FS::mv(std::string sourcepath, std::string destpath)
{
    // init varibles needed for the move
    u_int16_t ent_blk;
    u_int32_t ent_size;
    u_int8_t ent_rights, ent_type;
    std::string ent_name;
    std::string empty_name = "";
    int temp_dir = cur_dir; // set temp_dir to current directory
    std::vector<std::string> path_vec_s = path(sourcepath);
    std::string name_s = path_vec_s.back();
    path_vec_s.pop_back();

    int temp_dir3 = cur_dir; // To reset cur_dir after operations
    int dir_block_d;
    std::vector<std::string> path_vec_d = path(destpath);
    std::string name_d = path_vec_d.back();
    path_vec_d.pop_back();

    // checking for path to dest.
    if (destpath[0] == '/') 
    { 
        cur_dir = 0; 
    }
    int temp_dir2 = cur_dir;
    if (path_vec_d.size() > 0)
    {
        temp_dir2 = find_dir(path_vec_d);
    }
    cur_dir = temp_dir2;
    dir_block_d = find_index_dir(name_d);
    // Checking if the end of destpath is a file, dir or new name
    if (dir_block_d == -1)
    {
        dir_block_d = find_index(name_d);
        if (dir_block_d != -1) 
        {
            std::cout << "Directory not found or file already exists" << std::endl;
            return -1;            
        }
        ent_name = name_d;
    }
    else 
    {
        temp_dir2 = dir_block_d;
        ent_name = name_s;
    }



    if (path_vec_s.size() > 0) {
        temp_dir = find_dir(path_vec_s);
    }
    disk.read(temp_dir, reinterpret_cast<uint8_t *>(w_dir));

    for (auto &directory : w_dir)
    {
        if (directory.file_name == name_s && directory.type == TYPE_FILE)
        {
            if ((directory.access_rights & WRITE) == WRITE) 
            {
                ent_blk = directory.first_blk;
                ent_size = directory.size;
                ent_type = directory.type;
                ent_rights = directory.access_rights;
                std::strcpy(directory.file_name, empty_name.c_str());
                directory.type = EMPTY;
                directory.size = 0;
                directory.first_blk = 0;
                directory.access_rights = 0;
                break;
            }     
            else{
                print_anything("You do not have the correct access rights");
                return -1;
            }
        }
    }
    disk.write(temp_dir, reinterpret_cast<uint8_t *>(w_dir));

    disk.read(temp_dir2, reinterpret_cast<uint8_t *>(w_dir));
    for (dir_entry &entry : w_dir)
    {
        if (entry.type == EMPTY && ent_name != "/") {
            entry.first_blk = ent_blk;
            std::strcpy(entry.file_name, ent_name.c_str());
            entry.size = ent_size;
            entry.type = ent_type;
            entry.access_rights = ent_rights;
            break;
        }
    }
    disk.write(temp_dir2, reinterpret_cast<uint8_t *>(w_dir));
    cur_dir = temp_dir3; // set back cur_dir
    return 0;
}

// rm <filepath> removes / deletes the file <filepath>
int FS::rm(std::string filepath)
{
    int temp_dir = cur_dir;
    disk.read(FAT_BLOCK, reinterpret_cast<uint8_t *>(fat));
    std::vector<std::string> vec = path(filepath);
    std::string file_name = vec.back();

    vec.pop_back();

    if (vec.size() > 0) {
        temp_dir = find_dir(vec);
    }
    disk.read(temp_dir, reinterpret_cast<uint8_t *>(w_dir));

    int temp;
    int next_index;
    bool found = false;
    for (auto &directory : w_dir)
    {
        if (directory.file_name == file_name && directory.type == TYPE_FILE)
        {
            temp = find_index(file_name);
            found = true;
        }
        else if (directory.file_name == file_name && directory.type == TYPE_DIR) {
            disk.read(directory.first_blk, reinterpret_cast<uint8_t *>(w_dir));
            for (auto &dir : w_dir) {
                if (std::string(dir.file_name) != ".." && dir.type != EMPTY) {
                    std::cout << "Directory is not empty" << std::endl;
                    return -1;
                }
            }
            temp = find_index_dir(file_name);
            found = true;
        }
        if (found == true) {
            next_index = fat[temp];
            directory.type = EMPTY;
            directory.size = 0;
            directory.first_blk = 0;
            directory.access_rights = 0;
            break;
        }
    }
    if (found == false) {
        print_anything("File or dir does not exist");
        return -1;
    }
    // FREE fat blocks
    while (next_index != FAT_EOF)
    {
        next_index = fat[temp];
        fat[temp] = FAT_FREE;
        temp = next_index;
    }
    if (next_index == FAT_EOF)
    {
        fat[temp] = FAT_FREE;
    }


    disk.write(FAT_BLOCK, reinterpret_cast<uint8_t *>(fat));
    disk.write(temp_dir, reinterpret_cast<uint8_t *>(w_dir));

    return 0;
}

// append <filepath1> <filepath2> appends the contents of file <filepath1> to
// the end of file <filepath2>. The file <filepath1> is unchanged.
int FS::append(std::string filepath1, std::string filepath2)
{
    std::cout << "FS::append(" << filepath1 << "," << filepath2 << ")\n";
    // data block of filepath1
    char data_block[BLOCK_SIZE] = {0};
    char data_block_2[BLOCK_SIZE] = {0};
    disk.read(cur_dir, reinterpret_cast<uint8_t *>(w_dir));

    int filepath1_size = 0;
    int filepath2_size = 0;

    // Write data to the end of filepath2
    // Get index of filepath1 and filepath2
    int filepath1_index = find_index(filepath1);
    int filepath2_index = find_index(filepath2);

    disk.read(filepath1_index, reinterpret_cast<uint8_t *>(data_block));
    disk.read(filepath2_index, reinterpret_cast<uint8_t *>(data_block_2));
    // get size of filepath1 in w_dir
    int total_size = 0;
    for (auto &entry : w_dir)
    {
        if (entry.file_name == filepath1)
        {
            if ((entry.access_rights & READ) != READ)
            {
                print_anything("File is not readable");
                return -1;
            }
            filepath1_size = entry.size;
            total_size += entry.size;
        }
        if (entry.file_name == filepath2)
        {
            if ((entry.access_rights & WRITE) != WRITE)
            {
                print_anything("File is not writable");
                return -1;
            }

            filepath2_size = entry.size;
            total_size += entry.size; // update size of filepath2
        }
    }

    // update size of filepath2
    disk.read(cur_dir, reinterpret_cast<uint8_t *>(w_dir));
    for (auto &entry : w_dir)
    {
        if (entry.file_name == filepath2)
        {
            entry.size = total_size;
            disk.write(cur_dir, reinterpret_cast<uint8_t *>(w_dir));
        }
    }

    int counter = filepath2_size;
    for (int i = 0; i < filepath1_size; i++)
    {
        data_block_2[counter] = data_block[i]; // append data to the end of filepath2
        counter++;
    }
    // disk.write(cur_dir, reinterpret_cast<uint8_t *>(w_dir));
    disk.write(filepath1_index, reinterpret_cast<uint8_t *>(w_dir));
    disk.write(filepath1_index, reinterpret_cast<uint8_t *>(data_block));
    disk.write(filepath2_index, reinterpret_cast<uint8_t *>(data_block_2));
    return 0;
}

// mkdir <dirpath> creates a new sub-directory with the name <dirpath>
// in the current directory
int FS::mkdir(std::string dirpath)
{
    // Get the dir name and path to parent dir
    std::vector<std::string> path_vec = path(dirpath);
    std::string dir_name = path_vec.back();
    path_vec.pop_back();

    // Find the parent directory
    int parent_dir = cur_dir;
    if (!path_vec.empty())
    {
        parent_dir = find_dir(path_vec);
        if (parent_dir == -1)
        {
            print_anything("Directory not found");
            return -1;
        }
    }
    disk.read(parent_dir, reinterpret_cast<uint8_t *>(w_dir));
    // Check if name is too long
    if (dir_name.length() > 56)
    {
        print_anything("Directory name too long");
        return -1;
    }
    // check if dir already exists
    for (dir_entry entry : w_dir)
    {
        if (entry.file_name == dir_name)
        {
            print_anything("Directory or file with that name already exists");
            return -1;
        }
    }

    // Find a free block for the new directory
    int index = ff_blocks();
    if (index == -1)
    {
        print_anything("No space left on disk");
        return -1;
    }

    dir_entry new_dir;
    strcpy(new_dir.file_name, dir_name.c_str());
    new_dir.size = 0;
    new_dir.first_blk = index;
    new_dir.type = TYPE_DIR;
    new_dir.access_rights = READ | WRITE | EXECUTE;

    w_dir[index] = new_dir;

    // Update FAT table and parent directory block
    update_fat_table(new_dir.first_blk, sizeof(new_dir));

    disk.write(parent_dir, reinterpret_cast<uint8_t *>(w_dir));

    
    // Create dir_entry ".." first in this directory's block
    std::string filename = "..";
    std::strcpy(new_dir.file_name, filename.c_str());
    new_dir.size = 0;
    new_dir.first_blk = parent_dir; 
    new_dir.type = TYPE_DIR;
    new_dir.access_rights = READ | WRITE | EXECUTE;
    disk.read(index, reinterpret_cast<uint8_t *>(w_dir));
    w_dir[0] = new_dir;
    // Change dir_entry to be empty
    filename = "";
    std::strcpy(new_dir.file_name, filename.c_str());
    new_dir.first_blk = 0;
    new_dir.type = EMPTY;
    new_dir.access_rights = 0;

    // Fill rest of dir block with empty dir_entry
    for (int i = 1; i < BLOCK_SIZE / 64; i++)
    {
        w_dir[i] = new_dir;
    }

    disk.write(index, reinterpret_cast<uint8_t *>(w_dir));

    return 0;
}

//used to traverse a given path. Returns the correct block or -1 if it cant find it.
int FS::find_dir(std::vector<std::string> file_path)
{
    int dir_block = cur_dir;
    for (auto &dir_name : file_path) 
    {
        bool found = false;
        disk.read(dir_block, reinterpret_cast<uint8_t *>(w_dir));
        for (int i = 0; i < BLOCK_SIZE / 64; i++)
        {
            if (w_dir[i].type == TYPE_DIR && w_dir[i].file_name == dir_name)
            {
                dir_block = w_dir[i].first_blk;
                found = true;
            }
        }
        if (!found)
        {
            return -1;
        }
    }
    return dir_block;
}

// cd <dirpath> changes the current (working) directory to the directory named <dirpath>
int FS::cd(std::string dirpath)
{
    std::cout << "FS::cd(" << dirpath << ")\n";
    std::vector<std::string> path_vec = path(dirpath);

        if (dirpath[0] == '/') 
    { 
        cur_dir = 0; 
    }
    int block = find_dir(path_vec);
    if (block == -1)
    {
        std::cout << "Directory not found." << std::endl;
        return -1;
    }
    cur_dir = block;

    return 0;
}

int FS::pwd()
{
    std::string full_path = "";
    int temp_dir = cur_dir;
    disk.read(cur_dir, reinterpret_cast<uint8_t*>(w_dir));

    while (temp_dir != ROOT_BLOCK)
    {
        uint16_t pwd = w_dir[0].first_blk;
        disk.read(pwd, reinterpret_cast<uint8_t*>(w_dir));

        for (auto &dir : w_dir)
        {
            if (temp_dir == dir.first_blk)
            {
                full_path = dir.file_name + full_path;
                full_path = "/" + full_path;
                break;
            }
        }
        temp_dir = pwd;
    }

    if (full_path[0] != '/')
        full_path = "/" + full_path;

    std::cout << full_path << std::endl;

    return 0;
}

// chmod <accessrights> <filepath> changes the access rights for the
// file <filepath> to <accessrights>.
int FS::chmod(std::string accessrights, std::string filepath)
{
    disk.read(cur_dir, reinterpret_cast<uint8_t *>(w_dir));
    find_index(filepath);
    int r = stoi(accessrights); //typecast to int
    for (auto &entry : w_dir)
    {
        if (entry.file_name == filepath) //if filename is filepath. 
        {
            if (r == READ)
            {
                entry.access_rights = READ;
            }
            else if (r == WRITE)
            {
                entry.access_rights = WRITE;
            }
            else if (r == EXECUTE)
            {
                entry.access_rights = EXECUTE;
            }
            else if (r == READ | WRITE)
            {
                entry.access_rights = READ | WRITE;
            }
            else if (r == READ | EXECUTE)
            {
                entry.access_rights = READ | EXECUTE;
            }
            else if (r == WRITE | EXECUTE)
            {
                entry.access_rights = WRITE | EXECUTE;
            }
            else if (r == READ | WRITE | EXECUTE)
            {
                entry.access_rights = READ | WRITE | EXECUTE;
            }
            else
            {
                std::cout << "Invalid access rights" << std::endl;
                return -1;
            }
        }
        disk.write(cur_dir, (uint8_t *)w_dir);
    }
    return 0;
}