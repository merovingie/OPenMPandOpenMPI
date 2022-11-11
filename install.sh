  apt-get -y update && \
  DEBIAN_FRONTEND=noninteractive apt-get -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" upgrade && \
  DEBIAN_FRONTEND=noninteractive apt-get -y install \
  build-essential \
  mpich \
  tmux \
  git \
  zip \
  unzip && ln -s -f /usr/bin/python3 /usr/bin/python