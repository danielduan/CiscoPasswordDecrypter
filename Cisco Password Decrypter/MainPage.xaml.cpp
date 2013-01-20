//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <cstring>
#include <string>

using namespace Cisco_Password_Decrypter;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
	InitializeComponent();
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void) e;	// Unused parameter
}

int extract_index(char *string) {
  /* Function to extract the index into the known key from the hash string */

  int tens = 0;
  int ones = 0;
  int index = 0;

  tens = (*string - 48) * 10;
  string++;
  ones = *string - 48;
  index = tens + ones;
  return index;
}

char *string2bytes(char *string, char *hexbytes) {
  /* Take the hash string after extracting the index and convert it to an array of hex bytes */

  char hex;
  char ones;
  char cur;
  int length;
  int i;

  length = strlen(string);

  for (i = 0; i < length/2; i++) {
    if (*string <= 57 && *string >= 48)
      hex = (*(string) - 48) * 16;

    else
      hex = (*(string) - 55) * 16;

    string++;

    if (*string <= 57 && *string >= 48)
      ones = *(string) - 48;

    else
      ones = *(string) - 55;

    cur = hex + ones;
    string++;
    hexbytes[i] = cur;
  }
  hexbytes[i] = 0;
  return hexbytes;
}

String^ decrypt(Platform::String^ inputHash)
{
	/* Known Key */
  char ciscokey[] = {0x64, 0x73, 0x66, 0x64, 0x3b, 0x6b, 0x66, 0x6f, 0x41,
                     0x2c, 0x2e, 0x69, 0x79, 0x65, 0x77, 0x72, 0x6b, 0x6c,
                     0x64, 0x4a, 0x4b, 0x44, 0x48, 0x53, 0x55, 0x42};
  char hash[64];
  char hexhash[64];
  char password[64];
  char *hexhashptr;
  char *ptr;
  int index;
  int len;
  int i;
  int n;

  /* 0 out hexhash array */
  for (i = 0; i<64; i++){
    hexhash[i] = 0;
  }


  //extracts text from hash
  //fgets(hash, 63, inputHash);

	std::wstring hashwstring = inputHash->Data();
	for (i = 0; i < hashwstring.length(); i++) {
		hash[i] = hashwstring.at(i);
	}
	hash[i] = 0;

	if (hashwstring.length() < 4) {
		return "Invalid hash string";
	}
  
  ptr = hash;

  /* Extract 1st 2 characters of hash string, and convert them to decimal integer to be used as index into the key */

  index = extract_index(ptr);
  ptr += 2;
  len = (strlen(hash) / 2) - 1;
  hexhashptr = string2bytes(ptr,hexhash);
  for(i = 0; i < len; i++) {
	  int ckey = ciscokey[index];
	  int hkey = hexhashptr[i];
    password[i] = ciscokey[index] ^ hexhashptr[i];
	char result = password[i];
    index++;
	if (index >= 26) {
		index = 0;
	}
  }
  password[i] = 0;
  //printf("password: %s\n",password);


std::string s_str = std::string(password);
for (int x = 0; x < s_str.length(); x++) {
	if (!isdigit(s_str.at(x)) && !isalpha(s_str.at(x))) {
		return "Invalid hash string";
	}
}
std::wstring wid_str = std::wstring(s_str.begin(), s_str.end());
const wchar_t* w_char = wid_str.c_str();
Platform::String^ returnString = ref new Platform::String(w_char);

  return returnString;
}

void Cisco_Password_Decrypter::MainPage::textboxOutputPassword_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{

}


void Cisco_Password_Decrypter::MainPage::buttonDecrypt_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Platform::String^ input = textboxInputHash->Text;
	
	textboxOutputPassword->Text = decrypt(input);


}


void Cisco_Password_Decrypter::MainPage::textboxInputHash_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{

}
