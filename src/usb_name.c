#include "usb_names.h"

#define PRODUCT_NAME		{'C', 'l', 'u', 'b', 'S', 'p', 'o', 'r', 't', ' ', 'S','h','i','f','t','e','r',' ', 'S', 'Q', ' ', 'v', '1', '.', '5'}
#define PRODUCT_NAME_LEN	25

struct usb_string_descriptor_struct usb_string_product_name = {
  2 + PRODUCT_NAME_LEN * 2,
  3,
  PRODUCT_NAME
};

// Set usb_desc.h to #define PRODUCT_ID		0x0583