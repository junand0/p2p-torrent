/* 
    NXC Lab Torrent-esque P2P File Sharing System Assignment (2022 Fall) 
    Author: JS Park 

    torrent_functions.h is a header file for functions related to torrent data handling.
    Although the source code for the functions declared in this file is not provided,
    you still can (and should) use the functions in this header in your implementations 
    as long as use compile your main.c with torrent_functions.o (or torrent_functions_MAC.o).

    You are free to use any of the functions in this file for the final hand-in of your assignment.
*/

#ifndef TORRENT_FUNCTIONS_H
#define TORRENT_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#define STRING_LEN 128
#define MAX_FILE_SIZE (128*1024*1024ULL) // 128MiB
#define BLOCK_SIZE_DEFAULT (32*1024ULL) // 32KiB  
#define MAX_BLOCK_NUM (MAX_FILE_SIZE/BLOCK_SIZE_DEFAULT)
#define MAX_PEER_NUM 128
#define PEER_UPDATE_INTERVAL_MSEC_BASE 3000
#define PEER_EVICTION_REQ_NUM 5
#define MAX_TORRENT_NUM 512

typedef struct torrent_file torrent_file;
typedef struct torrent_info torrent_info;
extern torrent_file *global_torrent_list [MAX_TORRENT_NUM];
extern int num_torrents, peer_update_interval_msec;

// Struct for managing torrent data and associated information
struct torrent_file 
{
    char name[STRING_LEN];              // Name of the torrent
    unsigned int hash;                  // Hash value of the torrent

    unsigned int size;                  // Size of the torrent file (!= block_size*block_num)
    unsigned int downloaded_block_num;  // Number of downloaded blocks
    unsigned int block_num;             // Total number of blocks
    unsigned int block_size;            // Size of each block
    char block_info [MAX_BLOCK_NUM];    // Block info of the torrent (0: not downloaded, 1: downloaded)
    char *block_ptrs[MAX_BLOCK_NUM];    // Pointers to the start of each block
    char *data;                         // Pointer to the start of torrent data

    // A peer is defined by a unique <ip, port> pair, which is separately stored in peer_ip[]/port_port[] array.
    // The peer same index are used across all peer_[XXX] arrays for each unique peer.
    
    unsigned int num_peers;                 // Number of peers on this torrent
    char peer_ip[MAX_PEER_NUM][STRING_LEN]; // IP address of each peer
    int peer_port[MAX_PEER_NUM];            // Port number of each peer
    char peer_req_num[MAX_PEER_NUM];        // Number of pending requests to peers 
    char peer_block_info[MAX_PEER_NUM][MAX_BLOCK_NUM];  // Block info of each peer
};

// Struct for storing torrent information only.
struct torrent_info 
{
    char name[STRING_LEN];
    unsigned int hash;
    unsigned int size, block_num, block_size;
    char block_info [MAX_BLOCK_NUM];
};

// RETURN VALUES: Functions with integer return values return 0 on success, and -1 on failure, unless otherwise specified.

// Sleeps for milliseconds
void sleep_ms (int milliseconds);

// Returns the current time in milliseconds
unsigned int get_time_msec();

// Returns the 32-bit hash value of a data block (input the pointer & size of the block in bytes)
unsigned int hash(unsigned char *data, unsigned int size);

// Loads files and parses it into torrent_file struct. Torrent is added to global_torrent_list.
int make_file_into_torrent (char* file_name, char* input_file);

// Inits dynamic data structures in torrent_file struct 
// (All data initialized EXCEPT name, hash, size, block_num, block_size, block_info)
int init_torrent_dynamic_data (torrent_file *torrent);

// Saves torrent data to a file
int save_torrent_into_file (torrent_file *torrent, char *output_file);

// Prints torrent information
void print_torrent_info (torrent_file *torrent);

// Prints all torrent information in a global_torrent_list
void print_all_torrents ();

// Prints all torrent information in a global_torrent_list, in a compact form
void print_torrent_status ();

// Adds torrent to global_torrent_list
int add_torrent (torrent_file* torrent);

// Get pointer to the torrent from global_torrent_list from its hash value.
// Returns NULL if not found. Use this to check if a torrent is already in the list.
torrent_file *get_torrent (unsigned int torrent_hash);

// Frees dynamic memory of a torrent 
int free_torrent (torrent_file *torrent);

// Removes torrent from global_torrent_list
int remove_torrent (unsigned int torrent_hash);

// Removes peer from a torrent
int remove_peer_from_torrent (torrent_file *torrent, char *peer_ip, int peer_port);

// Returns the peer index from a torrent from its IP address and port number (-1 if not found)
int get_peer_idx (torrent_file *torrent, char *peer_ip, int peer_port);

// Adds peer to a torrent.
// Also updates peer's block info, if peer_block_info != NULL.
// If peer_block_info == NULL, it skips the block info update.
// Returns an error message and -1 if the peer is already in the torrent.
// Use get_peer_idx() to check if the peer is already in the torrent, before calling this function!
int add_peer_to_torrent (torrent_file *torrent, char* peer_ip, int peer_port, char* peer_block_info);

// Updates peer's block information in a torrent
void update_peer_block_info (torrent_file *torrent, char* peer_ip, int peer_port, char* block_info);

// Copies torrent information from torrent_info struct into torrent struct
int copy_info_to_torrent (torrent_file *torrent, torrent_info *info);

// Copies torrent information from torrent struct into torrent_info struct
int copy_torrent_to_info (torrent_file *torrent, torrent_info *info);

// Request torrent info from a peer, using the torrent's hash value
int request_from_hash (unsigned int torrent_hash, char *peer_ip, int peer_port);

#endif // TORRENT_FUNCTIONS_H