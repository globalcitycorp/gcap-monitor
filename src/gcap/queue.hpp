/*
 * pcap_base_processor.hpp
 * Copyright (C) 2021-21 - Globalciy, Corp.
 *
 * This project is using nDPI.
 *
 * Copyright (C) 2011-21 - ntop.org
 *
 * nDPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * nDPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with nDPI.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __GCAP_QUEUE_H__
#define __GCAP_QUEUE_H__

#include <condition_variable>
#include <mutex>
#include <queue>

namespace gcap {

template <typename T> class Queue {

  public:
    /**
     * Constructor
     */
    explicit Queue(size_t capacity) : capacity_(capacity) {}

    /**
     * Enqueue value
     */
    void enqueue(T val) {
        std::unique_lock<std::mutex> lk(guard_);
        not_full_.wait(lk, [this] { return queue_.size() < capacity_; });
        queue_.push(val);
        not_empty_.notify_all();
    }

    /**
     * Dequeue
     */
    T dequeue() {
        std::unique_lock<std::mutex> lock(guard_);
        not_empty_.wait(lock, [this] { return !queue_.empty(); });
        T ret = queue_.front();
        queue_.pop();
        not_full_.notify_all();
        return ret;
    }

  private:
    /**
     * Queue
     */
    std::queue<T> queue_;
    size_t capacity_;
    std::mutex guard_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
};

} // namespace gcap

#endif
