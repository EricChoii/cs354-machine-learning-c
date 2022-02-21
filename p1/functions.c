/******************************************************************************
* @file: functions.c
*
* WISC NETID			hchoi256
* CANVAS USERNAME		Eric Choi
* WISC ID NUMBER		9082005175
* OTHER COMMENTS FOR THE GRADER (OPTIONAL)
*
* @creator: Hojun Choi (hchoi256@wisc.edu)
* @modified: 09/25/2021
*****************************************************************************/
#include <stdio.h>
#include "functions.h"

// Some macros that may be useful to you 
#define MAX_USERNAME_LEN    32
#define MAX_EMAIL_LEN       32
#define MAX_DOMAIN_LEN      64
#define MIN_PASSWORD_LEN    8
#define MAX_PASSWORD_LEN    16
#define NEW_LINE_CHAR       10

// Set this to 1 to enable dbgprintf statements, make sure to set it back to 0 
// before submitting!
#define DEBUG               0
#define dbgprintf(...)      if (DEBUG) { printf(__VA_ARGS__); }

/******************************************************************************
* Helper functions
*****************************************************************************/

// ADD ANY HELPER FUNCTIONS YOU MIGHT WRITE HERE 
// Examples: IsLetter, IsDigit, Length, Find...

/*
* Check if the input is an alphabet or not.
*
* @param input :	A character
* @return			1 if valid, 0 if not
*/
int IsLetter(char input) {
	if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z'))
		return 1;
	else
		return 0;
}

/*
* Check if the input is a digit or not
*
* @param input  :	A character
* @return			1 if valid, 0 if not
*/
int IsDigit(char input) {
	if (input >= '0' && input <= '9')
		return 1;
	else
		return 0;
}

/*
* Return the string length
*
* @param arr :		A string
* @param len :		Size of the email input buffer
* @return			Length of a string
*/
int Length(char arr[], size_t len) {
	size_t c;

	for (c = 0; c < len; c++) {
		if (arr[c] == '\0')
			break;
	}

	return c;
}

/*
* Check if the string contains @ symbol or not
*
* @param arr :	A stinrg
* @param len :	Size of the email input buffer
* @param tar :	Target
* @return		1 if valid, 0 if not
*/
int Find(char arr[], size_t len, char tar) {
	size_t c = 0;

	for (; c < len; c++) {
		if (arr[c] == '\0')
			break;

		if (arr[c] == tar)
			return 1;
	}

	return 0;
}

/*
* Search the string that comes after the first @ symbol and
* save all domains in the two dimensional array 'domains'.
*
* @param email :	The email stinrg
* @param len :		Size of the email input buffer
* @param start :	Starting index for loop
* @param cmpt :		A two dimensional array of all components in the string
* @param symbol :	Whether symbol exists or not
* @return			Number of domains; -1: error
*/
int Get_Component(char email[], size_t len, char cmpt[512][512], int symbol) {
	int res = 0;			// number of domains, not including top-level domain

	for (size_t c = 0; c < len; c++) {	// find the end of name in the email string
		if (!symbol) {
			if (email[c] == '\0') {
				if (res == 0)
					res = c;

				break;
			}

			if (email[c] == '.')
				res = c;
		}
		else {
			if (email[c] == '@') {
				res = c;

				break;
			}
		}
	}

	for (int i = 0; i < res; i++) {
		if (i < res)
			cmpt[0][i] = email[i];
	}

	if (!symbol)	// error: symbol is missing
		return -1;

	size_t c = res + 1;	// set position after @ symbol
	res = 0;			// reset

	for (int i = 0, j = 1; c < len; c++) {	// get domains
		if (email[c] == '\0') {
			if (j == 1)		// example eric@edu
				return 1;	// # domain = 1 [edu]

			char tmp[512] = { '\0', };

			for (int k = 0; k < len; k++) {
				if ((cmpt[j][k] == '\0')) {
					cmpt[j][0] = '.';	// add '.' to top-level domain

					for (int l = 1; l < len; l++) {		// copy top-level domain
						if ((tmp[l - 1] == '\0'))
							break;

						cmpt[j][l] = tmp[l - 1];
					}

					break;
				}

				tmp[k] = cmpt[j][k];
			}
			break;
		}

		if (email[c] == '.') {
			res++, j++;
			i = 0;

			continue;
		}

		cmpt[j][i] = email[c];	// save each character of domain
		i++;
	}

	return res;
}

/*
* Compare string without using strcmp
*
* @param str1
* @param str2
* @return			1: same
*/
int stringCompare(char str1[], char str2[]) { // function definition
	int i = 0, flag = 0;
	while (str1[i] != '\0' && str2[i] != '\0') // until at least one string ends
	{
		if (str1[i] != str2[i])
		{
			flag = 1;
			break;
		}
		i++;
	}

	if (flag == 0 && str1[i] == '\0' && str2[i] == '\0')
		return 1;
	else
		return 0;
}

/******************************************************************************
* Verification functions
*****************************************************************************/

/*
* A username must begin with a letter [A-Z, a-z], contain 32 characters
* or less, and  may only consist of letters, underscores, or
* digits [A-Z, a-z, _, 0-9]. An error message is displayed if any of
* these conditions are not met. Only print the first applicable error
* message.
*
* @param user : The username string
* @param len : Size of the username input buffer
* @return 1 if valid, 0 if not
*/
int Verify_Username(char user[], size_t len) {
	int len_User_Name = 0;				// length of username

										// check if username begins with a letter [A-Z, a-z]
	if (!IsLetter(user[0])) {
		printf(ERROR_01_USER_START_INVALID);

		return 0;
	}

	// check if the length of username <= 32
	len_User_Name = Length(user, len);

	if (len_User_Name > MAX_USERNAME_LEN) {
		printf(ERROR_02_USER_LEN_INVALID);

		return 0;
	}

	// check if username only consists of letters, underscores, or digits [A-Z, a-z, _, 0-9]
	for (int i = 0; i < len_User_Name; i++) {
		if (user[i] == '\0')
			break;

		if (!(IsLetter(user[i]) || user[i] == '_' || IsDigit(user[i]))) {
			printf(ERROR_03_USER_CHARS_INVALID);

			return 0;
		}
	}

	printf(SUCCESS_1_USER);
	return 1;
}

/*
* An email address has four parts:
*      name
*          exists
*          must start with letter
*          max 32 characters
*          may contain only letters and digits
*      @ symbol
*          exists
*      domain name
*          exists
*          max of 64 characters
*          composed of one or more subdomains separated by .
*          subdomain must begin with a letter
*          subdomains may contain only letters and digits
*      top-level domain
*          must be [.edu, .com, .net]
*
* If the email address contains one or more errors print only the first
* applicable error from the list.
*
* Note this task is based on a real world problem and may not be the best
* order to approach writing the code.
*
* @param email : The email string
* @param len : Size of the email input buffer
* @return 1 if valid, 0 if not
*/
int Verify_Email(char email[], size_t len) {
	/* BEGIN MODIFYING CODE HERE */
	char cmpt[512][512] = { '\0', };	// email components; name [0], subdomains [1~end] and top-level domain [end]
	int nameLngth;						// length of email name
	int dmnLngth = -1;					// length of domain, not including top-level domain
	int dmnCount;						// # of subdomains
	int isTopLvl = 0;					// must be [.edu, .com, .net]; 1: yes, 0: no
	int isSymbl = 0;					// whether symbol exists; 1: yes, 0: no

										/**********
										** Name **
										**********/

										// check if name exists
	if (email[0] == '@') {
		printf(ERROR_04_EMAIL_MISSING_NAME);  // example @domain.com

		return 0;
	}

	// must start with letter
	if (!IsLetter(email[0])) {
		printf(ERROR_05_EMAIL_START_INVALID);

		return 0;
	}

	isSymbl = Find(email, len, '@');						// if @ symbol exists, return 1
	dmnCount = Get_Component(email, len, cmpt, isSymbl);	// store components and return number of domains 
	nameLngth = Length(cmpt[0], len);						// domains[0] = name

															// max 32 characters
	if (nameLngth > MAX_EMAIL_LEN) {
		printf(ERROR_06_EMAIL_NAME_LEN_INVALID);

		return 0;
	}

	// may contain only letters and digits
	for (int i = 0; i < nameLngth; i++) {
		if (!(IsLetter(email[i]) || IsDigit(email[i]))) {
			printf(ERROR_07_EMAIL_NAME_CHARS_INVALID);

			return 0;
		}
	}

	/**********
	* Symbol *
	**********/

	// check if symbol exists
	if (!isSymbl || dmnCount == -1) {
		printf(ERROR_08_EMAIL_MISSING_SYMBOL);

		return 0;
	}

	/**********
	* Domain *
	**********/

	// check if domain name exists
	if (dmnCount > 0) {
		if (cmpt[1][0] == '\0') {
			printf(ERROR_09_EMAIL_MISSING_DOMAIN); // example mike@.edu

			return 0;
		}
	}

	// max of 64 characters
	for (int i = 0; i < dmnCount; i++) {
		dmnLngth += Length(cmpt[i + 1], len);
		dmnLngth++;		// count '.'
	}

	if (dmnLngth > MAX_DOMAIN_LEN) {
		printf(ERROR_10_EMAIL_DOMAIN_LEN_INVALID);

		return 0;
	}

	// subdomain must begin with a letter
	for (int i = 0; i < dmnCount; i++) {
		if (!IsLetter(cmpt[i + 1][0])) {
			printf(ERROR_11_EMAIL_DOMAIN_START_INVALID);

			return 0;
		}
	}

	// subdomains may contain only letters and digits
	for (int i = 0; i < dmnCount; i++) {
		for (int j = 0; j < len; j++) {
			if (cmpt[i + 1][j] == '\0')
				break;


			if (!(IsLetter(cmpt[i + 1][j]) || IsDigit(cmpt[i + 1][j]))) {
				printf(ERROR_12_EMAIL_DOMAIN_CHARS_INVALID);

				return 0;
			}
		}
	}

	// must be [.edu, .com, .net]
	char* top_arr[3];

	top_arr[0] = ".edu";
	top_arr[1] = ".com";
	top_arr[2] = ".net";

	for (int i = 0; i < 3; i++) {
		isTopLvl = stringCompare(cmpt[dmnCount + 1], top_arr[i]);
		if (isTopLvl)
			break;
	}

	if (!isTopLvl) {
		printf(ERROR_13_TOP_LEVEL_DOMAIN_INVALID);

		return 0;
	}
	/* END MODIFYING CODE HERE */

	printf(SUCCESS_2_EMAIL);
	return 1;
}

/*
* The following password requirements must be verified:
*  - May use any character except spaces (i.e., "my password" is invalid)
*  - Must contain at least 8 characters (i.e., "Password" has 8 characters
*    and is valid)
*  - May have at most 16 characters (i.e., "1234567890Abcdef" has 16
*    characters and is valid)
*  - Must contain at least one upper case character [A-Z]
*  - Must contain at least one lower case character [a-z]
*
* @param pwd : The original password string
* @param len : Size of the original password input buffer
* @return 1 if valid, 0 if not
*/
int Verify_Password(char pwd[], size_t len) {

	/* BEGIN MODIFYING CODE HERE */
	int pwdLngth = 0;
	int	upperLetter = 0;
	int	lowerLetter = 0;

	// May use any character except spaces
	for (int i = 0; i < len; i++) {
		if (pwd[i] == '\0')
			break;

		if (pwd[i] == ' ') {
			printf(ERROR_14_PWD_SPACES_INVALID);

			return 0;
		}
	}

	// Must contain at least 8 characters
	for (; pwdLngth < len; pwdLngth++) {
		if (pwd[pwdLngth] == '\0')
			break;
	}

	if (pwdLngth < 8) {
		printf(ERROR_15_PWD_MIN_LEN_INVALID);

		return 0;
	}

	// May have at most 16 characters
	if (pwdLngth > 16) {
		printf(ERROR_16_PWD_MAX_LEN_INVALID);

		return 0;
	}

	// Must contain at least one upper case character
	for (int i = 0; i < len; i++) {
		if (pwd[i] == '\0')
			break;

		if (pwd[i] >= 'A' && pwd[i] <= 'Z') {
			upperLetter = 1;

			break;
		}
	}

	if (!upperLetter) {
		printf(ERROR_17_PWD_MIN_UPPER_INVALID);

		return 0;
	}

	// Must contain at least one lower case character
	for (int i = 0; i < len; i++) {
		if (pwd[i] == '\0')
			break;

		if (pwd[i] >= 'a' && pwd[i] <= 'z') {
			lowerLetter = 1;

			break;
		}
	}

	if (!lowerLetter) {
		printf(ERROR_18_PWD_MIN_LOWER_INVALID);

		return 0;
	}
	/* END MODIFYING CODE HERE */

	return 1;
}

/*
* Original Password and the Reentered Password must match
*
* @param pwd1 : The original password string
* @param len1 : Size of the original password input buffer
* @param pwd2 : The reentered password string
* @param len2 : Size of the renetered password input buffer
* @return 1 if valid, 0 if not
*/
int Verify_Passwords_Match(char pwd1[], size_t len1, char pwd2[], size_t len2) {
	/* BEGIN MODIFYING CODE HERE */
	if (!stringCompare(pwd1, pwd2)) {
		printf(ERROR_19_PWD_MATCH_INVALID);

		return 0;
	}
	/* END MODIFYING CODE HERE */

	printf(SUCCESS_3_PASSWORDS);
	return 1;
}

/******************************************************************************
* I/O functions
*****************************************************************************/

/*
* Prompts user with an input and reads response from stdin
*
* @param message : Prompt displayed to the user
* @param data : char array to hold user repsonse
* @param MAX_LENGTH : Size of user response input buffer
*/
void Get_User_Data(char *message, char *data, const int MAX_LENGTH) {
	printf("%s", message);
	fgets(data, MAX_LENGTH, stdin);

	/* BEGIN MODIFYING CODE HERE */
	for (int i = 0; i < MAX_LENGTH; i++) {
		if (data[i] == '\0')
			break;

		if (data[i] == '\n') {
			data[i] = '\0';

			break;
		}
	}
	/* END MODIFYING CODE HERE */
	return;
}