#include "FileHandler.h"


std::string FileHandler::getFileText(const std::string& file_path)
{
    // Open file
    std::ifstream in(file_path, std::ios_base::binary);

    if (!in)
    {
        throw std::logic_error("Error opening file!\n");
    }

    //Get length of file
    in.seekg(0, std::ios::end);
    size_t length = in.tellg();
    in.seekg(0, std::ios::beg);

    std::string buffer;
    buffer.reserve(length);

    std::copy(std::istreambuf_iterator<char>(in),
        std::istreambuf_iterator<char>(),
        std::back_inserter(buffer));

    return buffer; 
}

void FileHandler::pushTextToFile(const std::string& file_path, const std::string& text, const int& rank)
{
    // Open file
    // if rank == 0 than rewrite, append otherwise
    auto isAppend = std::ios::app;
    if (rank == 0)
        isAppend = std::ios::binary;
    std::ofstream out(file_path, isAppend);

    if (!out)
    {
        throw std::logic_error("Error opening file!\n");
    }

    out.write(text.c_str(), text.size());
}

void FileHandler::pushSize_tToFile(const std::string& file_path, const size_t& val, const int& rank)
{
    // Open file
    // if rank == 0 than rewrite, append otherwise
    auto isAppend = std::ios::app;
    if (rank == 0)
        isAppend = std::ios::binary;
    std::ofstream out(file_path, isAppend);

    if (!out)
    {
        throw std::logic_error("Error opening file!\n");
    }

    out.write(reinterpret_cast<const char*>(&val), sizeof(val));
}

std::vector<size_t> FileHandler::popSize_tFromFile(const std::string& file_path)
{
    std::ifstream file(file_path, std::ios::binary);
    if (!file)
    {
        throw std::logic_error("Error opening file!\n");
    }

    size_t val;
    std::vector<size_t> res;
    while (file.read(reinterpret_cast<char*>(&val), sizeof(val))) 
    {
        res.push_back(val);
        // Обработка считанного значения
    }
    file.close();

    return res;
}

int FileHandler::getFileSize(const std::string& file_path)
{
    std::ifstream file(file_path, std::ios::binary);

    if (!file)
    {
        std::string str = "Error opening file!\n";
        throw std::logic_error(str);
    }

    file.seekg(0, std::ios::end);
    int pos = file.tellg();
    file.close();
    return pos; // return current position of the stream (= file size)
}

void FileHandler::generateFileInfo(const std::string& file_path, const int& size)
{
    // Providing a seed value
    srand((unsigned)time(NULL));

    int rank, num_procs;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs); // get quantity of prosseses
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // rank of cur prosses

    if (rank == 0)
    {
        std::cout << "--- GENERATING file info using standard c++ rand() function ---\n";
        std::cout << "File path: " << file_path << "\n";
        std::cout << "Generating in process...\n";
    }

    // Рассчитываем размер каждой части файла
    const long long part_size = size / num_procs;
    const long long remainder = size % num_procs;

    // Cмещение для текущего процесса
    long long offset;

    // Вычисляем размер части файла для текущего процесса
    long long my_part_size = part_size;
    if (rank < remainder)
    {
        my_part_size++;
        offset = rank * part_size + rank;
    }
    else
    {
        offset = rank * part_size + remainder;
    }

    // DEBUG
    std::cout << "> Process  " << rank << ", part_size " << part_size << ", remainder " << remainder << ", my_part_size " << my_part_size << ", offset " << offset << "\n";

    // Генерируем часть файла параллельно
    std::string part;
    part.reserve(my_part_size);
    for (long long i = 0; i < my_part_size; i++)
    {
        int random = rand() % (ALFABET.size() - 1);// Get a random number (-1 because we save '\0')
        part.push_back(ALFABET.at(random));
    }

    // DEBUG
    std::cout << "> Process  " << rank << ": my_part_size (generated) = " << part.size() << "\n";
    MPI_Barrier(MPI_COMM_WORLD); // Ждем все процессы

    //---------------------------------
   
    // Процесс с рангом 0 очищает файл перед записью
    if (rank == 0)
    {
        // DEBUG
        std::cout << "> Process  0: clean file.txt\n";

        std::ofstream ofs;
        ofs.open(file_path, std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }

    MPI_Barrier(MPI_COMM_WORLD); // Дожидаемся всех процессов

    MPI_File file;
    // Открываем файл
    MPI_File_open(MPI_COMM_WORLD, file_path.c_str(), MPI_MODE_WRONLY | MPI_MODE_APPEND, MPI_INFO_NULL, &file);

    // Записываем данные в файл по очереди
    MPI_File_write_ordered(file, part.c_str(), part.size(), MPI_CHAR, MPI_STATUS_IGNORE);
    // DEBUG
    std::cout << "> Process  " << rank << ": appended my part to the file\n";

    // Закрываем файл
    MPI_File_close(&file);

    if (rank == 0)
    {
        std::cout << "DONE!\n";
        std::cout << "Text size after generating: " << size << '\n';
        std::cout << "---------------------------------\n";
    }
}

bool FileHandler::isFileValid(const std::string& file_path)
{
    std::ifstream file(file_path);
    char ch;

    if (!file)
        throw std::logic_error("Can't open a file!\n");

    while (file >> ch) // get char from file
    {
        if (ALFABET.has(ch) == false) // if char is not in the alfabet
        {
            file.close();
            return false; // then the whole file's info is incorrect
        }
    }
    file.close();
    return true;
}

bool FileHandler::areFilesEqual(const std::string& file1_path, const std::string& file2_path)
{
    if (getFileSize(file1_path) != getFileSize(file2_path))
    {
        return false;
    }

    std::ifstream file1(file1_path);
    std::ifstream file2(file2_path);

    if (file1 && file2)
    {
        char c1, c2;
        while (file1 >> c1 && file2 >> c2) // get char from file
        {
            if (c1 != c2)
            {
                file1.close();
                file2.close();
                return false;
            }
        }
        file1.close();
        file2.close();
        return true;
    }
    throw std::logic_error("Error opening files!");
}

void FileHandler::encodeFileUsingLZW(const std::string& file_path, const bool U)
{
    int rank, num_procs;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs); // get quantity of prosseses
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // rank of cur prosses
   
    // Process 0: print init info
    if (rank == 0)
    {
        std::cout << "--- ENCODING using LZW method ---\n";
        std::cout << "File path: " << file_path << "\n";

        // Validate file if not universal
        if (!U && !isFileValid(file_path))
        {
            std::string str = "The file is invalid according to the alphabet! (It has other symbols)\n";
            str += "Encoding using LZW can't be done!\nEncoding stopped.\n";
            str += "-------------------------------- - \n";
            throw std::logic_error(str);
        }
    }
    
    // All processes: Get info from file
    std::string fileText;
    try
    {
        for (int i_rank = 0; i_rank < num_procs; i_rank++)
        {
            if (rank == i_rank)
            {
                fileText = getFileText(file_path);
            }
            MPI_Barrier(MPI_COMM_WORLD); // Ждем все процессы
        }
        
    }
    catch (const std::exception& other)
    {
        std::string ex = other.what();
        ex += "Encoding using LZW can't be done!\nEncoding stopped.\n";
        ex += "-------------------------------- - \n";
        throw std::logic_error(ex);
    }

    // Рассчитываем размер каждой части файла
    const long long part_size = fileText.size() / num_procs;
    const long long remainder = fileText.size() % num_procs;
    // Cмещение для текущего процесса
    long long offset;
    // Вычисляем размер части файла для текущего процесса
    long long my_part_size = part_size;
    if (rank < remainder)
    {
        my_part_size++;
        offset = rank * part_size + rank;
    }
    else
    {
        offset = rank * part_size + remainder;
    }
    // DEBUG
    std::cout << "> Process  " << rank << ", part_size " << part_size << ", remainder " << remainder << ", my_part_size " << my_part_size << ", offset " << offset << "\n";

    // Выделяем память под локальный буфер для чтения данных
    std::vector<char> local_char_data(part_size);

    //------------------

    // DEBUG
    std::cout << "> Process  " << rank << ": fileText.size() = " << fileText.size() <<  "\n";

    // Process 0: print text size before encoding
    if (rank == 0)
    {
        std::cout << "Text size before encoding: " << fileText.size() << '\n';
        std::cout << "Encoding in process...\n";
    }
    
    // All processes: Encoding my part of a text
    // Encode File
    std::string encodedText;
    if (U) // universal method (not according the alphabet)
    {
        encodedText = LZWencoder_U::encode(fileText.substr(offset, my_part_size));
    }
    else // effective method (according the alphabet) 
    {
        encodedText = LZWencoder::encode(fileText.substr(offset, my_part_size));
    }

    // Save encoded info in file
    try
    {
        for (int i_rank = 0; i_rank < num_procs; i_rank++)
        {
            if (rank == i_rank)
            {
                pushTextToFile(file_path, encodedText, rank);
                std::cout << "> Process  " << rank << ": pushed my encoded part (size = " << encodedText.size() << ") to file\n";
                // save block size to file with name block_sizes.txt
                pushSize_tToFile(BLOCKSIZES_PATH, encodedText.size(), rank);
            }
            MPI_Barrier(MPI_COMM_WORLD); // Ждем все процессы
        }
    }
    catch (const std::exception& other)
    {
        std::string ex = other.what();
        ex += "Encoding using LZW can't be done!\nEncoding stopped.\n";
        ex += "-------------------------------- - \n";
        throw std::logic_error(ex);
    }

    MPI_Barrier(MPI_COMM_WORLD); // Ждем все процессы

    // Process 0: print DONE info
    if (rank == 0)
    {
        std::cout << "DONE!\n";
        /*std::cout << "Text size after encoding: " << encodedText.size() << '\n';
        std::cout << "Compression ratio: " << static_cast<double>(fileText.size()) / encodedText.size() << '\n';*/
        std::cout << "---------------------------------\n";
    }
}

void FileHandler::decodeFileUsingLZW(const std::string& file_path, const bool U)
{
    int rank, num_procs;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs); // get quantity of prosseses
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // rank of cur prosses

    std::vector<size_t> blocks_sizes = popSize_tFromFile(BLOCKSIZES_PATH);
    
    // DEBUG
    std::cout << "> Process  " << rank << ": block size saved = " << blocks_sizes[rank] << "\n";

    MPI_Barrier(MPI_COMM_WORLD); // Ждем все процессы

    //--------------------------
    
    if (rank == 0)
    {
        std::cout << "--- DECODING (LZW method) ---\n";
        std::cout << "File path: " << file_path << "\n";
    }

    // All processes: Get info from file
    std::string fileText;
    try
    {
        for (int i_rank = 0; i_rank < num_procs; i_rank++)
        {
            if (rank == i_rank)
            {
                fileText = getFileText(file_path);
            }
            MPI_Barrier(MPI_COMM_WORLD); // Ждем все процессы
        }
    }
    catch (const std::exception& other)
    {
        std::string ex = other.what();
        ex += "Encoding using LZW can't be done!\nEncoding stopped.\n";
        ex += "-------------------------------- - \n";
        throw std::logic_error(ex);
    }

    // DEBUG
    std::cout << "> Process  " << rank << ": have read from file = " << fileText.size() << "\n";

    // calc offset
    int offset(0);
    for (int i = 0; i < rank; i++)
    {
        offset += blocks_sizes[i];
    }

    // DEBUG
    std::cout << "> Process  " << rank << ": offset = " << offset << ", block size = " << blocks_sizes[rank] << "\n";

    ///-----------------

    if (rank == 0)
    {
        /*std::cout << "Text size before decoding: " << fileText.size() << '\n';*/
        std::cout << "Decoding in process...\n";
    }

    // Decode paralel
    std::string encodedText;
    try
    {
        if (U) // universal method (not according the alphabet)
        {
            encodedText = LZWencoder_U::decode(fileText.substr(offset, blocks_sizes[rank]));
        }
        else // effective method (according the alphabet) 
        {
            encodedText = LZWencoder::decode(fileText.substr(offset, blocks_sizes[rank]));
        }
    }
    catch (const std::exception&)
    {
        throw std::logic_error("\nPerhaps, this text isn't encoded using LZW!\nDecoding stopped!\n");
    }

    // Save decoded info in file
    try
    {
        for (int i_rank = 0; i_rank < num_procs; i_rank++)
        {
            if (rank == i_rank)
            {
                pushTextToFile(file_path, encodedText, rank);
                std::cout << "> Process  " << rank << ": pushed my encoded part (size = " << encodedText.size() << ") to file\n";
            }
            MPI_Barrier(MPI_COMM_WORLD); // Ждем все процессы
        }
        // change block sizes
        blocks_sizes[rank] = 0;
    }
    catch (const std::exception& other)
    {
        std::string ex = other.what();
        ex += "Decoding using LZW can't be done!\nDecoding stopped.\n";
        ex += "-------------------------------- - \n";
        throw std::logic_error(ex);
    }

    MPI_Barrier(MPI_COMM_WORLD); // Ждем все процессы

    if (rank == 0)
    {
        std::cout << "DONE!\n";
        /*std::cout << "Text size after decoding: " << encodedText.size() << '\n';
        std::cout << "Decompression ratio: " << static_cast<double>(fileText.size()) / encodedText.size() << '\n';*/
        std::cout << "------------------------------\n";
    }
}

void FileHandler::copyFile(const std::string& source_path, const std::string& destination_path)
{
    std::ifstream file(source_path);
    std::ofstream copy(destination_path);

    if (!file || !copy)
        throw std::logic_error("Can't open a file!\n");

    copy.clear();

    // Get info from file
    std::string fileText("");
    char ch;
    while (file.get(ch))
    {
        copy << ch;
    }

    copy.close();
    file.close();
}
