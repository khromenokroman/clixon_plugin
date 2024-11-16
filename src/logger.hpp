#pragma once

#include <systemd/sd-journal.h>

#include <string>

namespace custom_logger {
class Journal_interface {
   public:
    virtual ~Journal_interface() = default;
    virtual void print(uint8_t level, const std::string &message) = 0;
};

class Logger {
   public:
    explicit Logger(Journal_interface *journal, std::string name_lgger)
        : m_name_logger(std::move(name_lgger)), m_root_logger(nullptr), m_journal(journal){};

    Logger(Journal_interface *journal, Logger const &root, std::string name_lgger)
        : m_name_logger(std::move(name_lgger)), m_root_logger(&root), m_journal(journal){};

    void write(uint8_t level, std::string const &message) {
        if (level <= m_log_level) {
            m_journal->print(level, get_child_name_logger() + " " + message);
        }
    }

    Logger(Logger const &) = delete;

    void set_level(uint8_t level) { m_log_level = level; }

    [[nodiscard]] uint8_t get_level() const { return m_log_level; }

    [[nodiscard]] std::string get_child_name_logger() const {
        std::string s;
        Logger const *root = m_root_logger;
        while (root) {
            s += "[" + root->m_name_logger + "] ";
            root = root->m_root_logger;
        }
        s += "[" + m_name_logger + "]";
        return s;
    }

   private:
    std::string m_name_logger;
    Logger const *m_root_logger;
    uint8_t m_log_level = LOG_INFO;
    Journal_interface *m_journal;
};

class Systemd_journal final : public custom_logger::Journal_interface {
   public:
    void print(uint8_t level, const std::string &message) final override {
        sd_journal_print(level, "%s", message.c_str());
    }
};
} // namespace custom_logger