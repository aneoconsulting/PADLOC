#ifndef UTILS_BARRIER_HEADER
#define UTILS_BARRIER_HEADER

/**
 * \file utils.hpp
 * \brief Library Manipulation API Sample, part of the Padloc project.
 * Utilitary header, containing various functions for modes and log levels.
 * 
 * \author Brasseur Dylan, Teaudors Mickaël, Valeri Yoann
 * \date 2019
 * \copyright Interflop 
 */
#include <fstream>

/**
 * \def UNKNOWN_ARGUMENT
 * \brief Macro giving the amount of parsers behind the main parser function.
 * \details If the macro is equal to 5, that means there are 5 plugins, and if 
 * the error count is equal to 5 after checking all the parsers, that means the
 * option is unknown.
 */
#define UNKNOWN_ARGUMENT 3

/**
 * \def MAX
 * \brief Computes the maximum between two number x and y.
 */
#define MAX(x, y) ((x) > (y) ? (x) : (y))

/**
 * \enum padloc_symbol_mode_t
 * \brief Specifies the mode for the symbol plugin
 * \details Specifies all the mode available for the symbol plugin,
 * regarding the use of Blacklists, Whitelists and Generation.
 */
typedef enum{
    /** Default */
	PLC_SYMBOL_DEFAULT = 0,
    /** Don't look at symbols */
    PLC_SYMBOL_NOLOOKUP = 0,
    /** Generate the symbols from an execution */
    PLC_SYMBOL_GENERATE,
    /** Don't instrument the symbols in the blacklist */
    PLC_SYMBOL_BL_ONLY,
    /** Instrument only the symbols in the whitelist */
    PLC_SYMBOL_WL_ONLY,
    /** Instrument the symbols in the whitelist that aren't in the blacklist */
    PLC_SYMBOL_BL_WL, 
    /** Display arguments help */
    PLC_SYMBOL_HELP
} padloc_symbol_mode_t;

/**
 * \enum padloc_analyse_mode_t
 * \brief Specifies the mode for the backend analysis plugin
 * \details Specifies all the mode for backend analysis. Currently, only 2
 * are implemented : analysis needed and not needed. If the analysis was
 * already when parsing the command line arguments, analysis is not
 * needed anymore, thus the PLC_ANALYSE_NOT_NEEDED mode.
 * \todo Add more modes for the backend analysis, to analyse the backend after
 * parsing the command line, instead of at the same time.
 */
typedef enum{
    /** Backend analysis not needed */
    PLC_ANALYSE_NOT_NEEDED,
    /** Backend analysis needed */
    PLC_ANALYSE_NEEDED
} padloc_analyse_mode_t;

/**
 * \brief Setter for the log level
 * 
 * \param level The new log level
 */
void set_log_level(int level);

/**
 * \brief Getter for the log level
 * \return The current log level
 */
int get_log_level();

/**
 * \brief Setter for the client mode
 * 
 * \param mode The new client mode
 */
void set_symbol_mode(padloc_symbol_mode_t mode);

/**
 * \brief Getter for the client mode
 * \return The current client mode
 */
padloc_symbol_mode_t get_symbol_mode();

/**
 * \brief Setter for the backend analysis mode
 * 
 * \param mode The new backend analysis mode
 */
void set_analyse_mode(padloc_analyse_mode_t mode);

/**
 * \brief Getter for the backend analysis mode
 * \return The current backend analysis mode
 */
padloc_analyse_mode_t get_analyse_mode();

/**
 * \brief Helper function for printing the help string, when a command line
 * related bug occurs, or the user uses "-h" or "--help".
 */
void print_help();

/**
 * \brief Writes to an output file the symbol file header, needed for
 * the symbol analysis part of the program.
 * 
 * \param output The output file in which to write
 */
void write_to_file_symbol_file_header(std::ofstream& output);

/**
 * \brief Incrementer for the error count.
 * \details When a parser function doesn't recognize an option, it calls
 * this function to increment the error count. That way, if when all the
 * parser have been called and none recognizes the option, the error count
 * is equal to the number of parsers, and an error is triggered.
 */
void inc_error();

/**
 * \brief Helper function to that check if a string is a number
 * 
 * \param str The string to check
 * \return True if the string represents a number
 */
bool is_number(const std::string& str);

/**
 * \brief Main parsing function
 * \details Get each argument of the command line, and calls all the parsers
 * for the other parts of the program (currently utilitary, backend analysis
 * and symbol analysis). The called functions are functions of the form :
 * bool name(std::string arg, int *i, int argc, char* argv[]).
 * The second parameter is given as pointer so that the parsing functions
 * can modify it if they see fit, for instance when they detect an option
 * that needs another argument.
 * 
 * The arguments that we detect are the ones given by DynamoRIO, meaning it
 * is not the actual command line, but the options listed after the 
 * "-c library" up until the "-- application".
 * Generally, the command line for DynamoRIO with drrun will look like this :
 * "drrun DynamoRIO_option -c client client_options -- app app_options"
 * So what we check in this function is the client_options part.
 * 
 * \param argc The number of arguments
 * \param argv The arguments of the command line
 * 
 * \return True if the execution of the program must stop, else false
 * 
 * \todo Get the global parser to use the obverser pattern, in order to be
 * more flexible on the parser possible, be able to remove the call to
 * parsers and actually specifying the interface for the functions. Same for
 * the plugin managers.
 * 
 * \todo Have different type of return possible for the parsers, in order to 
 * remove the error counter (maybe 3 outputs possible, one for when a parser
 * identified a command option, one for when a it didn't recognized the option,
 * and one for when there was a problem).
 */
bool arguments_parser(int argc, const char* argv[]);

#endif