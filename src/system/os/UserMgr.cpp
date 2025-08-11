#include "os/UserMgr.h"
#include "obj/Dir.h"

UserMgr *TheUserMgr;

UserMgr::UserMgr() : unk2c(true) { SetName("user_mgr", ObjectDir::Main()); }

void UserMgr::GetLocalUsers(std::vector<LocalUser *> &users) const {
    std::vector<User *> allUsers;
    GetUsers(allUsers);
    for (int i = 0; i < allUsers.size(); i++) {
        if (allUsers[i]->IsLocal()) {
            users.push_back(allUsers[i]->GetLocalUser());
        }
    }
}
