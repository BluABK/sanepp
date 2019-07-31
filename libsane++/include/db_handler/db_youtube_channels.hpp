#ifndef SANE_DB_YOUTUBE_CHANNELS_HPP
#define SANE_DB_YOUTUBE_CHANNELS_HPP

#include <iostream>
#include <vector>
#include <list>

#include <entities/youtube_channel.hpp>
#include <db_handler/db_handler.hpp>

#define NO_ERROR_LOG nullptr

namespace sane {
    /**
     * Adds a list of YoutubeChannel objects to an SQLite3 Database.
     *
     * Conflict handling: If an entry already exists it will be overwritten with the new values.
     *
     * @param t_channels
     * @return
     */
    int addChannelsToDB(const std::list <std::shared_ptr<YoutubeChannel>>& t_channels,
                        std::list<std::string> *t_errors);
} // namespace sane

#endif //SANE_DB_YOUTUBE_CHANNELS_HPP
