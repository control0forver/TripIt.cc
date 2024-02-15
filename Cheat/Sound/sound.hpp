#pragma once
#include <dsound.h>

#include <deque>
#include <mutex>
#include <condition_variable>

class CSound {
   public:
    typedef LPDIRECTSOUND8 SoundHinstance;
    typedef LPDIRECTSOUNDBUFFER SoundResc;
    typedef const char* SoundID;
    typedef std::vector<SoundResc> SoundData;
    typedef std::map<SoundID, SoundData> SoundsCollection;

    static bool IsSoundExists(const SoundsCollection& source, const SoundID& sound) {
        const auto& it = source.find(sound);

        if (it == source.end()) return false;

        return it->second.size();
    }

   public:
    template <typename T>
    class ConcurrentQueue {
       public:
        explicit ConcurrentQueue(size_t maxSize) : maxSize(maxSize) {}

        bool empty() { return queue_.empty(); }
        bool full() { return queue_.size() >= maxSize; }

        void push(const T& item, bool concurrent = true) {
            std::unique_lock<std::mutex> lock(mutex_);
            while (full()) {
                if (!concurrent) return;

                notFullCond.wait(lock);
            }
            queue_.push_back(item);
            notEmptyCond.notify_one();
        }

        T pop(bool concurrent = true) {
            std::unique_lock<std::mutex> lock(mutex_);
            while (queue_.empty()) {
                if (!concurrent) return {};

                notEmptyCond.wait(lock);
            }
            T frontItem = queue_.front();
            queue_.pop_front();
            notFullCond.notify_one();
            return frontItem;
        }

       protected:
        std::deque<T> queue_;
        std::mutex mutex_;
        std::condition_variable notEmptyCond;
        std::condition_variable notFullCond;
        size_t maxSize;
    };

    class SoundController final : public ConcurrentQueue<SoundResc> {
        size_t BufferQueueSize;
        size_t BufferCopysPerSound;

       public:
        size_t GetBufferQueueSize() { return BufferQueueSize; }
        size_t GetBufferCopysPerSound() { return BufferCopysPerSound; }

       public:
        SoundController(size_t BufferQueueSize, size_t BufferCopysPerSound = 50) : ConcurrentQueue(BufferQueueSize) {
            this->BufferQueueSize = BufferQueueSize;
            this->BufferCopysPerSound = BufferCopysPerSound;
        }
        ~SoundController() {
            queue_.clear();
            m_Sounds.clear();
        }

        void QueuePlayAll();
        void QueuePopSound(size_t pops, bool waitForEmpty);
        void QueuePushSound(SoundID id, bool waitForFull);

        const SoundsCollection& GetSounds() { return m_Sounds; }
        void AddSound(SoundID id, const SoundData& data);
        void RemoveSound(SoundID id);

       private:
        SoundsCollection m_Sounds;

        size_t m_Index = 0;
    };

   private:
    void InitializeDirectSound(HWND window);
    void ReleaseDirectSound();

   public:
    static CSound& Get() {
        static CSound inst;
        return inst;
    }

    ~CSound();

    void Initialize(HWND window);
    void Shutdown();

    bool LoadSoundFromMemory(SoundController& controller, SoundID id, const void* data, size_t dataSize);
    bool LoadSoundFromFile(SoundController& controller, SoundID id, const char* file);

    void FreeSound(SoundID id);
    void FreeAllSounds();

    const SoundHinstance& GetSoundHinstance();
    const SoundsCollection& GetGlobalSounds();

   private:
    SoundHinstance m_hSound;          // Sound device instance
    SoundsCollection m_GlobalSounds;  // Store all sounds of instance, for finally free
};
