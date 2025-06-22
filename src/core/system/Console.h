//
// Created by exozg on 24/01/2025.
//

#ifndef CONSOLE_H
#define CONSOLE_H

namespace Msml::Core::System {
    class Console {
        bool mIsEnabled = false;

    public:
        void Enable();

        void Disable();

        ~Console();
    };
}

#endif //CONSOLE_H
