#include <iostream>
#include "constants.h"
#include "FileHandler.h"
#include "UserInterface.h"

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv); // set up mpi

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size); // get quantity of prosseses

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); // rank of cur prosses

	FileHandler f;
	UserInterface ui;
	int input;
	char input_to_continue = 'c';

	while (input_to_continue != 'q' && input_to_continue != 'Q')
	{
		if (world_rank == 0)
		{
			std::cout << "<---- MAIN MENU OF THE PROGRAM ---->\n";
			std::cout << "(0) Generate file (FILE_PATH) info\n";
			std::cout << "(1) Copy file to COPY_PATH\n";
			std::cout << "(2) Compare files (FILE_PATH and COPY_PATH)\n";
			std::cout << "----------------------------\n";
			std::cout << "LZW method\n";
			std::cout << "  LZW (universal): \n";
			std::cout << "    (3) Encode LZW_u\n";
			std::cout << "    (4) Decode LZW_u\n";
			std::cout << "  LZW (according to the ALPHABET) This's more effective:\n";
			std::cout << "    (5) Encode LZW\n";
			std::cout << "    (6) Decode LZW\n";
			std::cout << "\nInput your option: ";

			input = ui.inputInt();
		}

		MPI_Bcast(&input, 1, MPI_INT, 0, MPI_COMM_WORLD); // Send val to other nodes

		if (world_rank == 0)
			std::cout << "\n";

		try
		{
			switch (input)
			{
			case 0:
				f.generateFileInfo(FILE_PATH, FILE_SIZE);
				break;
			case 1:
				if (world_rank == 0)
				{
					f.copyFile(FILE_PATH, COPY_PATH);
					std::cout << "File (FILE_PATH) has been copied to a copy file (COPY_PATH)\n";
				}
				break;
			case 2:
				if (world_rank == 0)
				{
					if (f.areFilesEqual(FILE_PATH, COPY_PATH))
					{
						if (world_rank == 0)
							std::cout << "Files are equal.\n";
					}
					else
					{
						if (world_rank == 0)
							std::cout << "Files are different!\n";
					}
				}
				break;
			case 3:
				f.encodeFileUsingLZW(FILE_PATH, true);
				break;
			case 4:
				f.decodeFileUsingLZW(FILE_PATH, true);
				break;
			case 5:
				f.encodeFileUsingLZW(FILE_PATH, false);
				break;
			case 6:
				f.decodeFileUsingLZW(FILE_PATH, false);
				break;
			default:
				if (world_rank == 0)
					std::cout << "There is no such option in the menu! Try again.\n";
				break;
			}
		}
		catch (const std::exception& ex)
		{
			if (world_rank == 0)
				std::cout << ex.what();
		}

		MPI_Barrier(MPI_COMM_WORLD); // Ждем все процессы

		if (world_rank == 0)
		{
			std::cout << "\nTo quit press \'q\' or \'Q\'. To continue press any other symbol...\n";

			input_to_continue = ui.inputLetter();
		}
			
		
		MPI_Bcast(&input_to_continue, 1, MPI_CHAR, 0, MPI_COMM_WORLD); // Send val to other nodes
	}

	MPI_Finalize(); // End mpi
	return 0;
}
