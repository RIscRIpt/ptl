#pragma once

#include <cassert>
#include <memory>
#include <stdexcept>
#include <variant>
#include <vector>

namespace ptl {

    template<typename T>
    class Array {
    private:
        struct SegmentTree {
            std::shared_ptr<void> lson = nullptr;
            std::shared_ptr<void> rson = nullptr;
        };

        class Value {
        public:
            operator T() const { return value_; }

            Value operator=(T const &new_value)
            {
                array_.set(index_, new_value);
                return Value(array_, index_, new_value);
            }

            Value(Array<T> &array, size_t index, T const &value)
                : array_(array)
                , index_(index)
                , value_(value)
            {
            }

        private:
            Array<T> &array_;
            size_t index_;
            T const &value_;
        };

    public:
        Array(size_t size, T const &default_value = T())
            : size_(size)
        {
            auto tree =
                std::static_pointer_cast<SegmentTree>(history_.emplace_back(
                    std::move(std::make_shared<SegmentTree>())));
            make_tree(*tree, 0, size, default_value);
        }

        T const &operator[](size_t index) const { return get(index); }
        Value operator[](size_t index)
        {
            return Value(*this, index, get(index));
        }
        size_t get_history_size() const { return history_.size(); }
        T const &get(size_t index, size_t history_id = -1)
        {
            if (index >= size_) {
                throw std::out_of_range("index out of range");
            }
            if (history_id == -1) {
                history_id = history_.size() - 1;
            }
            auto tree =
                std::static_pointer_cast<SegmentTree>(history_[history_id]);
            size_t begin = 0;
            size_t end = size_;
            while (true) {
                size_t middle = begin + (end - begin) / 2;
                if (index < middle) {
                    end = middle;
                    if (begin < end - 1) {
                        tree = std::static_pointer_cast<SegmentTree>(tree->lson);
                    }
                    else {
                        return *std::static_pointer_cast<T>(tree->lson);
                    }
                }
                else {
                    begin = middle;
                    if (begin < end - 1) {
                        tree = std::static_pointer_cast<SegmentTree>(tree->rson);
                    }
                    else {
                        return *std::static_pointer_cast<T>(tree->rson);
                    }
                }
            }
        }

        void set(size_t index, T const &new_value)
        {
            if (index >= size_) {
                throw std::out_of_range("index out of range");
            }
            auto tree = std::static_pointer_cast<SegmentTree>(history_.back());
            auto new_tree =
                std::static_pointer_cast<SegmentTree>(history_.emplace_back(
                    std::move(std::make_shared<SegmentTree>())));
            size_t begin = 0;
            size_t end = size_;
            while (true) {
                size_t middle = begin + (end - begin) / 2;
                if (index < middle) {
                    end = middle;
                    new_tree->rson = tree->rson;
                    if (begin < end - 1) {
                        new_tree->lson =
                            std::move(std::make_shared<SegmentTree>());
                        tree =
                            std::static_pointer_cast<SegmentTree>(tree->lson);
                        new_tree = std::static_pointer_cast<SegmentTree>(
                            new_tree->lson);
                    }
                    else {
                        new_tree->lson =
                            std::move(std::make_shared<T>(new_value));
                        break;
                    }
                }
                else {
                    begin = middle;
                    new_tree->lson = tree->lson;
                    if (begin < end - 1) {
                        new_tree->rson = std::make_shared<SegmentTree>();
                        tree =
                            std::static_pointer_cast<SegmentTree>(tree->rson);
                        new_tree = std::static_pointer_cast<SegmentTree>(
                            new_tree->rson);
                    }
                    else {
                        new_tree->rson =
                            std::move(std::make_shared<T>(new_value));
                        break;
                    }
                }
            }
        }

    private:
        void make_tree(SegmentTree &tree,
                       size_t begin,
                       size_t end,
                       T const &default_value)
        {
            auto middle = begin + (end - begin) / 2;
            if (begin < middle - 1) {
                auto lson = std::make_shared<SegmentTree>();
                tree.lson = lson;
                make_tree(*lson, begin, middle, default_value);
            }
            else {
                tree.lson = std::make_shared<T>(default_value);
            }
            if (middle < end - 1) {
                auto rson = std::make_shared<SegmentTree>();
                tree.rson = rson;
                make_tree(*rson, middle, end, default_value);
            }
            else {
                tree.rson = std::make_shared<T>(default_value);
            }
        }

        size_t size_;
        std::vector<std::shared_ptr<void>> history_;
    };

}
