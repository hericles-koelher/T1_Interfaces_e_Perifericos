#include <linux/init.h>				// Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>			// Core header for loading LKMs into the kernel
#include <linux/device.h>			// Header to support the kernel Driver Model
#include <linux/kernel.h>			// Contains types, macros, functions for the kernel
#include <linux/fs.h>				// Header for the Linux file system support
#include <linux/uaccess.h>			// Required for the copy to user function
#include <linux/mutex.h>			// Required for the mutex functionality
#include <linux/ctype.h>			// Help handling input
#include <linux/types.h>			// Required for int32_t

#define  DEVICE_NAME "mycalc"		// The device will appear at /dev/mycalc using this value
#define  CLASS_NAME  "char"			// The device class -- this is a character device driver

MODULE_LICENSE("GPL");              // The license type -- this affects available functionality
// MODULE_AUTHOR("*******"); // The author -- visible when you use modinfo
MODULE_DESCRIPTION("A simple Linux char driver calculator module");  // The description -- see modinfo
MODULE_VERSION("0.1");              // A version number to inform users

#define STR_INT32 12

static int		majorNumber;					// Stores the device number -- determined automatically
static char		expression_string[256] = {0};	// Memory for the string that is passed from userspace
static char		result_string[STR_INT32] = {0};	// Memory for the string that is passed to userspace
static char		firstOperand[STR_INT32] = {0};
static char		secondOperand[STR_INT32] = {0};
static char		op;
static struct	class*  myCalcClass  = NULL;	// The device-driver class struct pointer
static struct	device* myCalcDevice = NULL;	// The device-driver device struct pointer

static 	DEFINE_MUTEX(myCalcMutex);

static void get_result(void);
static void set_data(void);

static int		dev_open(struct inode *, struct file *);
static int		dev_release(struct inode *, struct file *);
static ssize_t	dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t	dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init mycalc_init(void){
    printk(KERN_INFO "MyCalc: Initializing the MyCalc driver as LKM\n");

    // Initialize the mutex lock dynamically at runtime
    mutex_init(&myCalcMutex);

    // Try to dynamically allocate a major number for the device -- more difficult but worth it
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber<0){
        printk(KERN_ALERT "MyCalc failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "MyCalc: registered correctly with major number %d\n", majorNumber);

    // Register the device class
    myCalcClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(myCalcClass)){                   // Check for error and clean up if there is
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(myCalcClass);            // Correct way to return an error on a pointer
    }
    printk(KERN_INFO "MyCalc: device class registered correctly\n");

    // Register the device driver
    myCalcDevice = device_create(myCalcClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(myCalcDevice)){                 // Clean up if there is an error
        class_destroy(myCalcClass);              // Repeated code but the alternative is goto statements
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(myCalcDevice);
    }
    printk(KERN_INFO "MyCalc: device class created correctly\n"); // Made it! Device was initialized
    return 0;
}

static void __exit mycalc_exit(void){
    mutex_destroy(&myCalcMutex);							// destroy the dynamically-allocated mutex
    device_destroy(myCalcClass, MKDEV(majorNumber, 0));		// remove the device
    class_unregister(myCalcClass);							// unregister the device class
    class_destroy(myCalcClass);								// remove the device class
    unregister_chrdev(majorNumber, DEVICE_NAME);			// unregister the major number
    printk(KERN_INFO "MyCalc: Goodbye!\n");
}

static int dev_open(struct inode *inodep, struct file *filep){
	if(!mutex_trylock(&myCalcMutex)){
		// Try to acquire the mutex (i.e., put the lock on/down)
		// returns 1 if successful and 0 if there is contention
		printk(KERN_ALERT "MyCalc: Device in use by another process");
		return -EBUSY;
	}
	return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
    int error_count = 0;
    // copy_to_user has the format ( * to, *from, size) and returns 0 on success
    get_result();
    error_count = copy_to_user(buffer, result_string, strlen(result_string));

    if (error_count==0){                // if true then have success
        printk(KERN_INFO "MyCalc: Sent %ld characters to the user\n", strlen(result_string));
        return 0;  // clear the position to the start and return 0
    }
    else {
        printk(KERN_INFO "MyCalc: Failed to send %d characters to the user\n", error_count);
        return -EFAULT;               // Failed -- return a bad address expression_string (i.e. -14)
    }
}

static void get_result(void){
	int32_t fOp, sOp, result;

	kstrtoint(firstOperand, 10, &fOp);
	kstrtoint(secondOperand, 10, &sOp);

	switch(op){
		case '+':
			result = fOp + sOp;
			break;
		case '-':
           	result = fOp - sOp;
           	break;
		case '*':
			result = fOp * sOp;
			break;
		case '/':
           	result = fOp / sOp;
           	break;
	}

	sprintf(result_string, "%d", result);
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
    strcpy(expression_string, buffer);
	set_data();

    printk(KERN_INFO "MyCalc: Received %zu characters from the user\n", len);
    return len;
}

static void set_data(void){
	int i = 0;

	// The first char may be a signal or digit.
	// If it's a signal it will cause some trouble
	// with the for loop...
	firstOperand[i] = expression_string[i];

	for(; isdigit(expression_string[i]); i++){
		firstOperand[i] = expression_string[i];
	}

	firstOperand[i]='\0';
	op = expression_string[i++];

	for(; expression_string[i]!='\0'; i++){
		secondOperand[i] = expression_string[i];
	}

    secondOperand[i]='\0';

	printk(KERN_INFO "Expression: %s %c %s\n", firstOperand, op, secondOperand);
}

static int dev_release(struct inode *inodep, struct file *filep){
    mutex_unlock(&myCalcMutex);          // Releases the mutex (i.e., the lock goes up)
    printk(KERN_INFO "MyCalc: Device successfully closed!\n");
    return 0;
}

module_init(mycalc_init);
module_exit(mycalc_exit);
