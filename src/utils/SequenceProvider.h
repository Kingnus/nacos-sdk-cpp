#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "NacosString.h"
#include "NacosExceptions.h"
#include "thread/AtomicInt.h"
#include "src/thread/Mutex.h"
#include "src/config/IOUtils.h"

namespace nacos
{

template<typename T>
class SequenceProvider {
private:
    NacosString _fileName;
    AtomicInt<T> _current;
    Mutex _acquireMutex;
    T _nr_to_preserve;
    T _initSequence;
    volatile T _hwm;//high water mark

    void ensureWrite(int fd, T data) {
        size_t bytes_written = 0;
        while (bytes_written < sizeof(T)) {
            bytes_written += write(fd, (char*)&data + bytes_written, sizeof(T) - bytes_written);
        }
    }

    void preserve() {
        T current;
        int fd;
        bool newFile = false;
        if (IOUtils::checkNotExistOrNotFile(_fileName)) {
            newFile = true;
        }
        fd = open(_fileName.c_str(), O_RDWR | O_CREAT);
        if (fd <= 0) {
            log_debug("errno = %d", errno);
            throw new NacosException(NacosException::UNABLE_TO_OPEN_FILE, _fileName);
        }
        
        if (newFile) {
            ensureWrite(fd, _initSequence);
            lseek(fd, 0, SEEK_SET);//read from the beginning
        }

        size_t bytes_read = 0;
        while (bytes_read < sizeof(T))
        {
            bytes_read += read(fd, (char*)&current + bytes_read, sizeof(T) - bytes_read);
        }
        std::cout << "got from file:" << current << std::endl;
        lseek(fd, 0, SEEK_SET);//write from the beginning

        ensureWrite(fd, current + _nr_to_preserve);
        close(fd);
        _current.set(current);
        _hwm = current + _nr_to_preserve;
    };
public:
    SequenceProvider(const NacosString &fileName, T initSequence, T nr_to_preserve) {
        _fileName = fileName;
        _initSequence = initSequence;
        _nr_to_preserve = nr_to_preserve;
    };

    T next(int step = 1) {
        T res = _current.getAndInc(step);
        while (res >= _hwm) {
            _acquireMutex.lock();
            if (res >= _hwm) {
                preserve();
            }
            _acquireMutex.unlock();
        }
        return res;
    };
};

} // namespace nacos
