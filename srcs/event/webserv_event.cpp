#include "webserv_event.hpp"
#include "global.hpp"
#include "socket_reader.hpp"
#include <sys/types.h>
#include <sys/wait.h>

void WebservEvent::update_time()
{
    DEBUG("update_time()");
    this->updated_time_ = std::time(NULL);
}

std::time_t WebservEvent::last_updated_time()
{
    return (this->updated_time_);
}

bool WebservEvent::check_timeout(std::time_t now)
{
    DEBUG("WebservEvent::check_timeout diff=" + Utility::to_string(now - this->updated_time_));
    std::time_t diff = now - this->updated_time_;
    return (diff >= TIMEOUT);
}

bool WebservEvent::check_died_child()
{
    DEBUG("WebservEvent::check_died_child");
    if(this->entity_ && this->entity_->app_result() && this->entity_->app_result()->is_cgi()){
        int wstatus = 0;
        waitpid(this->entity_->app_result()->pid().to_int(), &wstatus,  WUNTRACED | WCONTINUED | WNOHANG);

        if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) != 0) {
            MYINFO("WebservEvent::check_died_child True");
            this->entity_->app_result()->clear_pid();
            if(this->entity_->io().get_write_fd() > 0){
                this->entity_->io().get_write_fd().close();
                this->entity_->io().set_write_fd(FileDiscriptor());
            }
            if(this->entity_->io().get_read_fd() > 0){
                this->entity_->io().get_read_fd().close();
                this->entity_->io().set_read_fd(FileDiscriptor());
            }

            return (true);
        }
    }
    MYINFO("WebservEvent::check_died_child False");
    return (false);
}
